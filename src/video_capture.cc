/******************************************************************************
 * Filename:    video_capture.cpp
 * Description: Video capture task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_capture.h"

#include <atomic>
#include <iostream>

#include "error_handling.h"

extern std::atomic<bool> shutting_down;

VideoCapture::VideoCapture()
    : task_(TaskId::VIDEO_CAPTURE, TaskPriority::VIDEO_CAPTURE,
            TaskUpdatePeriodMs::VIDEO_CAPTURE, VideoCaptureFunction) {
    task_.SetData(this);
    StartCapture();
}

VideoCapture::~VideoCapture() {
    StopCapture();
    std::cout << "Shutting down video capture\n";
}

void VideoCapture::StartCapture() {
    capture_.open(0);
    if (!capture_.isOpened()) {
        throw VideoCaptureException("Failed to start video capture");
    }
}
void VideoCapture::StopCapture() {
    try {
        if (capture_.isOpened()) {
            capture_.release();
        }
    } catch (const std::exception& e) {
        std::string error_message =
            "Error removing diagnostics folder: " + std::string(e.what());
        throw VideoCaptureException(error_message);
    }
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
