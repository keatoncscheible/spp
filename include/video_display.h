/******************************************************************************
 * Filename:    video_display.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_DISPLAY_H
#define VIDEO_DISPLAY_H

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
    void DisplayVideo();
    Task task_;
    VideoProcessing& video_processing_;
};

#endif  // VIDEO_DISPLAY_H