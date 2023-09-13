/******************************************************************************
 * Filename:    video_processor.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_processor.h"

#include <atomic>
#include <iostream>
#include <mutex>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_task.h"

VideoProcessor::VideoProcessor(
    VideoTask& input,
    std::shared_ptr<VideoTransformerFactory> transformer_factory, TaskId id,
    TaskPriority priority, std::atomic<bool>& shutting_down)
    : VideoTask(id, priority, TaskUpdatePeriodMs(0), TaskFcn, shutting_down),
      input_(input),
      transformer_factory_(transformer_factory) {
    transformer_ = transformer_factory_->Create();
    task_.SetData(this);
}

VideoProcessor::~VideoProcessor() {}

void VideoProcessor::ChangeTransformer(
    std::shared_ptr<VideoTransformerFactory> new_transformer_factory) {
    transformer_factory_ = new_transformer_factory;
    transformer_ = transformer_factory_->Create();
}

void VideoProcessor::GetInputFrame(cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(input_.mutex_);
    input_.cond_.wait(lock);
    input_.GetOutputFrame(frame);
}

void VideoProcessor::ProcessFrame(cv::Mat& frame) {
    transformer_->Transform(frame);
}

void VideoProcessor::TaskFcn(Task* task) {
    VideoProcessor* self = static_cast<VideoProcessor*>(task->GetData());

    while (!self->shutting_down_) {
        cv::Mat& frame = self->next_buffer_;
        self->GetInputFrame(frame);
        self->ProcessFrame(frame);
        if (!frame.empty()) {
            self->SwapBuffers();
            self->NotifyListeners();
        } else {
            std::cerr << "Processor read an empty frame." << std::endl;
        }
    }
    self->NotifyListeners();
}
