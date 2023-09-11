/******************************************************************************
 * Filename:    video_process.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESS_H
#define VIDEO_PROCESS_H

#include <condition_variable>
#include <opencv2/opencv.hpp>

#include "task.h"
#include "video_capture.h"

class VideoProcess {
   public:
    VideoProcess(VideoCapture& video_capture_);
    ~VideoProcess();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    cv::Mat* GetFrame() { return current_buffer_; }
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void VideoProcessFunction(Task* task);
    cv::Mat& ProcessVideo(cv::Mat& frame);
    Task task_;
    VideoCapture& video_capture_;
    cv::Mat buffer1_, buffer2_;
    cv::Mat* current_buffer_ = &buffer1_;
    cv::Mat* next_buffer_ = &buffer2_;
};

#endif  // VIDEO_PROCESS_H
