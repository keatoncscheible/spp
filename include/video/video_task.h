/******************************************************************************
 * Filename:    video_task.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_TASK_H
#define VIDEO_TASK_H

#include <atomic>
#include <condition_variable>
#include <mutex>

#include "opencv2/core.hpp"
#include "task.h"

class VideoTask {
   public:
    VideoTask(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
              TaskFunction function, std::atomic<bool>& shutting_down);
    virtual ~VideoTask();
    void Init();
    void GetOutputFrame(cv::Mat& frame);
    void Shutdown();
    std::mutex mutex_;
    std::condition_variable cond_;

   protected:
    void SwapBuffers();
    void NotifyListeners();
    void Throttle();
    Task task_;
    std::atomic<bool>& shutting_down_;
    cv::Mat buffer1_, buffer2_;
    cv::Mat& current_buffer_;
    cv::Mat& next_buffer_;
};

#endif  // VIDEO_TASK_H