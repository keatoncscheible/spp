/******************************************************************************
 * Filename:    VIDEO_OUTPUT.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include <opencv2/opencv.hpp>

#include "task.h"
#include "video_processing.h"

class VideoDisplay {
   public:
    VideoDisplay(VideoProcessing& video_processing_);
    ~VideoDisplay();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }

   private:
    const char* kWindowName = "Video";
    static void VideoDisplayFunction(Task* task);
    void GetInputFrame(cv::Mat& frame);
    void DisplayVideo(cv::Mat& frame);
    void ThrottleDisplay();
    Task task_;
    VideoProcessing& video_processing_;
};

#endif  // VIDEO_OUTPUT_H