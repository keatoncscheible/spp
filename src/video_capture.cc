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
            TaskUpdatePeriodMs::VIDEO_CAPTURE, VideoCaptureFunction),
      current_buffer_(buffer1_),
      next_buffer_(buffer2_) {
    task_.SetData(this);
    StartCapture();
}

VideoCapture::~VideoCapture() {
    StopCapture();
    std::cout << "Shutting down video capture\n";
}

void VideoCapture::StartCapture() {
    if (capture_.isOpened()) {
        return;
    }
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
            "Error stopping video capture: " + std::string(e.what());
        throw VideoCaptureException(error_message);
    }
}

void VideoCapture::VideoCaptureFunction(Task* task) {
    VideoCapture* self = static_cast<VideoCapture*>(task->GetData());

    while (!shutting_down) {
        self->capture_ >> (self->next_buffer_);

        if (self->next_buffer_.empty()) {
            std::cerr << "Failed to capture frame." << std::endl;
            continue;
        }
        {
            std::lock_guard<std::mutex> lock(self->mutex_);
            std::swap(self->current_buffer_, self->next_buffer_);
        }
        self->cond_.notify_one();
    }
}
