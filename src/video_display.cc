/******************************************************************************
 * Filename:    video_display.cpp
 * Description: Video display task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_display.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "video_processing.h"

extern std::atomic<bool> shutting_down;

VideoDisplay::VideoDisplay(VideoProcessing& video_processing)
    : video_processing_(video_processing),
      task_(TaskId::VIDEO_DISPLAY, TaskPriority::VIDEO_DISPLAY,
            TaskUpdatePeriodMs::VIDEO_DISPLAY, VideoDisplayFunction) {
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
        auto start = std::chrono::high_resolution_clock::now();
        self->DisplayVideo();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        int sleep_time = self->task_.period_ms.count() - elapsed;

        if (sleep_time > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        };
    }
}

void VideoDisplay::DisplayVideo() {
    cv::Mat frame;
    {
        std::unique_lock<std::mutex> lock(video_processing_.mutex_);
        video_processing_.cond_.wait(lock);
        frame = video_processing_.GetFrame();
    }
    cv::imshow(kWindowName, frame);
    cv::waitKey(1);
}