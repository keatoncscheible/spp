/******************************************************************************
 * Filename:    video_process.cpp
 * Description: Video processing task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_process.h"

#include <atomic>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "video_capture.h"

extern std::atomic<bool> shutting_down;

VideoProcess::VideoProcess(VideoCapture& video_capture)
    : video_capture_(video_capture),
      task_(TaskId::VIDEO_PROCESS, TaskPriority::VIDEO_PROCESS,
            TaskUpdatePeriodMs::VIDEO_PROCESS, VideoProcessFunction) {
    task_.SetData(this);
}

VideoProcess::~VideoProcess() {
    std::cout << "Shutting down video processing\n";
}

void VideoProcess::VideoProcessFunction(Task* task) {
    VideoProcess* self = static_cast<VideoProcess*>(task->GetData());

    cv::Mat frame;
    while (!shutting_down) {
        {
            std::unique_lock<std::mutex> lock(self->video_capture_.mutex_);
            self->video_capture_.cond_.wait(lock);
            frame = *(self->video_capture_.GetFrame());
        }

        *(self->next_buffer_) = self->ProcessVideo(frame);

        {
            std::lock_guard<std::mutex> lock(self->mutex_);
            std::swap(self->current_buffer_, self->next_buffer_);
        }

        self->cond_.notify_one();
    }
}

cv::Mat& VideoProcess::ProcessVideo(cv::Mat& frame) {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
    return frame;
}
