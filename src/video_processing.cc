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
#include "timing.h"
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
        self->GetInputFrame(frame);
        auto processing_time = TimeFunction(
            [&]() { self->ProcessVideo(frame, self->next_buffer_); });
        self->time_stats_.Push(processing_time);
        self->SwapBuffers();
        self->NotifyListeners();
    }
}

void VideoProcessing::GetInputFrame(cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(video_capture_.mutex_);
    video_capture_.cond_.wait(lock);
    frame = video_capture_.GetOutputFrame();
}

void VideoProcessing::ProcessVideo(cv::Mat& frame_in, cv::Mat& frame_out) {
    if (frame_in.empty()) {
        std::cerr << "Cannot process empty frame\n";
        return;
    }
    cv::cvtColor(frame_in, frame_out, cv::COLOR_BGR2HLS);
}

void VideoProcessing::SwapBuffers() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::swap(current_buffer_, next_buffer_);
}

void VideoProcessing::NotifyListeners() { cond_.notify_one(); }