/******************************************************************************
 * Filename:    video_capture.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <condition_variable>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "task.h"

class VideoCapture {
   public:
    VideoCapture();
    ~VideoCapture();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    cv::Mat& GetFrame() { return current_buffer_; }
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void VideoCaptureFunction(Task* task);
    cv::VideoCapture capture_;
    void StartCapture();
    void StopCapture();
    Task task_;

    cv::Mat buffer1_, buffer2_;
    cv::Mat& current_buffer_;
    cv::Mat& next_buffer_;
};

#endif  // VIDEO_CAPTURE_H
