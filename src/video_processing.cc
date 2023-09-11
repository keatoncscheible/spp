/******************************************************************************
 * Filename:    video_processing.cpp
 * Description: Video processing task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_processing.h"

#include <atomic>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "error_handling.h"
#include "video_capture.h"

extern std::atomic<bool> shutting_down;

VideoProcessing::VideoProcessing(VideoCapture& video_capture)
    : video_capture_(video_capture),
      task_(TaskId::VIDEO_PROCESSING, TaskPriority::VIDEO_PROCESSING,
            TaskUpdatePeriodMs::VIDEO_PROCESSING, VideoProcessingFunction),
      current_buffer_(buffer1_),
      next_buffer_(buffer2_) {
    task_.SetData(this);
}

VideoProcessing::~VideoProcessing() {
    std::cout << "Shutting down video processing\n";
}

void VideoProcessing::VideoProcessingFunction(Task* task) {
    VideoProcessing* self = static_cast<VideoProcessing*>(task->GetData());

    cv::Mat frame;
    while (!shutting_down) {
        {
            std::unique_lock<std::mutex> lock(self->video_capture_.mutex_);
            self->video_capture_.cond_.wait(lock);
            frame = self->video_capture_.GetFrame();
        }

        (self->next_buffer_) = self->ProcessVideo(frame);

        {
            std::lock_guard<std::mutex> lock(self->mutex_);
            std::swap(self->current_buffer_, self->next_buffer_);
        }

        self->cond_.notify_one();
    }
}

cv::Mat& VideoProcessing::ProcessVideo(cv::Mat& frame) {
    if (frame.empty()) {
        std::cerr << "Cannot process empty frame\n";
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
    return frame;
}
