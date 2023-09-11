/******************************************************************************
 * Filename:    video_capture.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <condition_variable>
#include <opencv2/opencv.hpp>

#include "task.h"

class VideoCapture {
   public:
    VideoCapture();
    ~VideoCapture();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    static void VideoCaptureFunction(Task* task);
    cv::Mat* GetCurrentFrame();
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    cv::VideoCapture capture_{0};
    Task task_;
    cv::Mat buffer1_, buffer2_;
    cv::Mat* current_buffer_ = &buffer1_;
    cv::Mat* next_buffer_ = &buffer2_;
};

#endif  // VIDEO_CAPTURE_H
