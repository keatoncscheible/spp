/******************************************************************************
 * Filename:    video_output.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_output.h"

#include <atomic>
#include <iostream>
#include <mutex>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_task.h"

VideoOutput::VideoOutput(VideoTask& input,
                         std::shared_ptr<VideoConsumerFactory> consumer_factory,
                         TaskId id, TaskPriority priority,
                         TaskUpdatePeriodMs update_period,
                         std::atomic<bool>& shutting_down)
    : VideoTask(id, priority, update_period, TaskFcn, shutting_down),
      input_(input),
      consumer_factory_(consumer_factory) {
    consumer_ = consumer_factory_->Create();
    task_.SetData(this);
}

VideoOutput::~VideoOutput() {}

void VideoOutput::ChangeConsumer(
    std::shared_ptr<VideoConsumerFactory> new_consumer_factory) {
    consumer_factory_ = new_consumer_factory;
    consumer_ = consumer_factory_->Create();
}

void VideoOutput::GetInputFrame(cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(input_.mutex_);
    input_.cond_.wait(lock);
    input_.GetOutputFrame(frame);
}

void VideoOutput::OutputFrame(cv::Mat& frame) { consumer_->Consume(frame); }

void VideoOutput::TaskFcn(Task* task) {
    VideoOutput* self = static_cast<VideoOutput*>(task->GetData());

    cv::Mat frame;
    while (!self->shutting_down_) {
        self->GetInputFrame(frame);
        if (!frame.empty()) {
            self->OutputFrame(frame);
        } else {
            std::cerr << "Output read an empty frame." << std::endl;
        }
    }
}
