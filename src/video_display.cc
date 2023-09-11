/******************************************************************************
 * Filename:    video_display.cpp
 * Description: Video display task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_display.h"

#include <atomic>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "video_process.h"

extern std::atomic<bool> shutting_down;

VideoDisplay::VideoDisplay(VideoProcess& video_process)
    : video_process_(video_process),
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
        self->DisplayVideo();
    }
}

void VideoDisplay::DisplayVideo() {
    VideoDisplay* self = static_cast<VideoDisplay*>(task_.GetData());

    cv::Mat frame;
    {
        std::unique_lock<std::mutex> lock(self->video_process_.mutex_);
        self->video_process_.cond_.wait(lock);
        frame = *(self->video_process_.GetFrame());
    }
    cv::imshow(self->kWindowName, frame);
    cv::waitKey(1);
}