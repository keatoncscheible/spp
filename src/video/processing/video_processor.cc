/******************************************************************************
 * Filename:    video_processor.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_processor.h"

#include <atomic>
#include <iostream>
#include <mutex>

#include "logger.h"
#include "opencv2/core.hpp"
#include "task.h"
#include "video_task.h"

VideoProcessor::VideoProcessor(
    VideoTask& input,
    std::shared_ptr<VideoTransformerFactory> transformer_factory, TaskId id,
    TaskPriority priority, std::atomic<bool>& shutting_down)
    : VideoTask(id, priority, TaskUpdatePeriodMs(0), TaskFcn, shutting_down),
      input_(input),
      transformer_factory_(transformer_factory),
      running_(false) {
    transformer_ = transformer_factory_->Create();
    task_.SetData(this);
}

VideoProcessor::~VideoProcessor() {}

void VideoProcessor::Start() { running_ = true; }

void VideoProcessor::Stop() { running_ = false; }

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
    auto start_time = std::chrono::high_resolution_clock::now();
    transformer_->Transform(frame);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                               end_time - start_time)
                               .count();
    auto elapsed_time = static_cast<double>(elapsed_time_ns) * 1.0e-9;
    time_stats_.Push(elapsed_time);
}

void VideoProcessor::TaskFcn(Task* task) {
    VideoProcessor* self = static_cast<VideoProcessor*>(task->GetData());

    while (!self->shutting_down_) {
        if (self->running_) {
            cv::Mat& frame = self->next_buffer_;
            self->GetInputFrame(frame);
            if (!frame.empty()) {
                self->ProcessFrame(frame);
                self->SwapBuffers();
                self->NotifyListeners();
            } else {
                spdlog::error("Processor received an empty frame.");
            }
        }
    }
    self->NotifyListeners();
}
