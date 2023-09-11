/******************************************************************************
 * Filename:    video_capture.cpp
 * Description: Video capture task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_capture.h"

#include <atomic>
#include <iostream>

extern std::atomic<bool> shutting_down;

VideoCapture::VideoCapture()
    : task_(TaskId::VIDEO_CAPTURE, TaskPriority::VIDEO_CAPTURE,
            TaskUpdatePeriodMs::VIDEO_CAPTURE, VideoCaptureFunction) {
    task_.SetData(this);

    if (!capture_.isOpened()) {
        std::cerr << "Error: Cannot open webcam!" << std::endl;
        return;
    }
}

VideoCapture::~VideoCapture() {
    capture_.release();
    std::cout << "Shutting down video capture\n";
}

void VideoCapture::VideoCaptureFunction(Task* task) {
    VideoCapture* self = static_cast<VideoCapture*>(task->GetData());

    while (!shutting_down) {
        self->capture_ >> *(self->next_buffer_);

        if (self->next_buffer_->empty()) {
            std::cerr << "Failed to capture frame." << std::endl;
            continue;
        }
        self->mutex_.lock();
        std::swap(self->current_buffer_, self->next_buffer_);
        self->mutex_.unlock();
        self->cond_.notify_one();
    }
}

cv::Mat* VideoCapture::GetCurrentFrame() { return current_buffer_; }
