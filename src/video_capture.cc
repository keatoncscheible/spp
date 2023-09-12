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
extern std::condition_variable capture_cv;
extern std::mutex capture_mutex;

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
        auto frame_captured = self->CaptureFrame();
        if (!frame_captured) {
            continue;
        }
        self->SwapBuffers();
        self->NotifyListeners();
        self->ThrottleCapture();
    }
    self->NotifyListeners();
}

bool VideoCapture::CaptureFrame() {
    capture_ >> next_buffer_;

    if (!next_buffer_.empty()) {
        return true;
    }
    std::cerr << "Failed to capture frame." << std::endl;
    return false;
}

void VideoCapture::SwapBuffers() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::swap(current_buffer_, next_buffer_);
}

void VideoCapture::NotifyListeners() { cond_.notify_one(); }

void VideoCapture::ThrottleCapture() {
    std::unique_lock<std::mutex> lock(capture_mutex);
    capture_cv.wait_for(lock, task_.period_ms);
}
