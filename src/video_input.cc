/******************************************************************************
 * Filename:    video_input.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_input.h"

#include <atomic>
#include <iostream>
#include <mutex>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_task.h"

VideoInput::VideoInput(std::shared_ptr<VideoSourceFactory> source_factory,
                       TaskId id, TaskPriority priority,
                       TaskUpdatePeriodMs update_period,
                       std::atomic<bool>& shutting_down)
    : VideoTask(id, priority, update_period, TaskFcn, shutting_down),
      source_factory_(source_factory) {
    source_ = source_factory_->Create();
    task_.SetData(this);
}

VideoInput::~VideoInput() {}

void VideoInput::Init() {
    source_->Open();
    VideoTask::Init();
}

void VideoInput::Start() { source_->Open(); }

void VideoInput::Stop() { source_->Close(); }

void VideoInput::Shutdown() {
    source_->Close();
    VideoTask::Shutdown();
}

void VideoInput::GetInputFrame(cv::Mat& frame) { source_->ReadFrame(frame); }

void VideoInput::ChangeSource(
    std::shared_ptr<VideoSourceFactory> new_source_factory) {
    Stop();
    source_factory_ = new_source_factory;
    source_ = source_factory_->Create();
    Start();
}

void VideoInput::TaskFcn(Task* task) {
    VideoInput* self = static_cast<VideoInput*>(task->GetData());

    while (!self->shutting_down_) {
        cv::Mat& frame = self->next_buffer_;
        self->GetInputFrame(frame);
        if (!frame.empty()) {
            self->SwapBuffers();
            self->NotifyListeners();
        } else {
            std::cerr << "input frame is empty" << std::endl;
        }

        self->Throttle();
    }
    self->NotifyListeners();
}
