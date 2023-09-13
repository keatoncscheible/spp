/******************************************************************************
 * Filename:    VIDEO_OUTPUT.cpp
 * Description: Video display task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "VIDEO_OUTPUT.h"
#include "video_processing.h"

extern std::atomic<bool> shutting_down;
extern std::condition_variable display_cv;
extern std::mutex display_mutex;

VideoDisplay::VideoDisplay(VideoProcessing& video_processing)
    : video_processing_(video_processing),
      task_(TaskId::VIDEO_OUTPUT, TaskPriority::VIDEO_OUTPUT,
            TaskUpdatePeriodMs::VIDEO_OUTPUT, VideoDisplayFunction) {
    task_.SetData(this);
}

VideoDisplay::~VideoDisplay() {
    cv::destroyWindow(kWindowName);
    std::cout << "Shutting down video display\n";
}

void VideoDisplay::VideoDisplayFunction(Task* task) {
    VideoDisplay* self = static_cast<VideoDisplay*>(task->GetData());

    cv::namedWindow(self->kWindowName);

    cv::Mat frame;
    while (!shutting_down) {
        self->GetInputFrame(frame);
        self->DisplayVideo(frame);
        self->ThrottleDisplay();
    }
}

void VideoDisplay::GetInputFrame(cv::Mat& frame) {
    std::unique_lock<std::mutex> lock(video_processing_.mutex_);
    video_processing_.cond_.wait(lock);
    frame = video_processing_.GetOutputFrame();
}

void VideoDisplay::DisplayVideo(cv::Mat& frame) {
    cv::imshow(kWindowName, frame);
    cv::waitKey(1);
}

void VideoDisplay::ThrottleDisplay() {
    std::unique_lock<std::mutex> lock(display_mutex);
    display_cv.wait_for(lock, task_.period_ms_);
}