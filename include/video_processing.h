/******************************************************************************
 * Filename:    video_processing.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESSING_H
#define VIDEO_PROCESSING_H

#include <condition_variable>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "task.h"
#include "video_capture.h"

class VideoProcessing {
   public:
    VideoProcessing(VideoCapture& video_capture_);
    ~VideoProcessing();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    cv::Mat& GetFrame() { return current_buffer_; }
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void VideoProcessingFunction(Task* task);
    cv::Mat& ProcessVideo(cv::Mat& frame);
    Task task_;
    VideoCapture& video_capture_;
    cv::Mat buffer1_, buffer2_;
    cv::Mat& current_buffer_;
    cv::Mat& next_buffer_;
};

#endif  // VIDEO_PROCESSING_H
