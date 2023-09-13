/******************************************************************************
 * Filename:    video_task.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_task.h"

#include <atomic>

#include "task.h"

VideoTask::VideoTask(TaskId id, TaskPriority priority,
                     TaskUpdatePeriodMs period_ms, TaskFunction function,
                     std::atomic<bool>& shutting_down)
    : task_(id, priority, period_ms, function),
      shutting_down_(shutting_down),
      current_buffer_(buffer1_),
      next_buffer_(buffer2_) {}

VideoTask::~VideoTask() {}

void VideoTask::Init() { task_.Start(); }

void VideoTask::Shutdown() { task_.Join(); }

void VideoTask::GetOutputFrame(cv::Mat& frame) { frame = current_buffer_; }

void VideoTask::SwapBuffers() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::swap(current_buffer_, next_buffer_);
}

void VideoTask::NotifyListeners() { cond_.notify_one(); }

void VideoTask::Throttle() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait_for(lock, task_.period_ms);
}
