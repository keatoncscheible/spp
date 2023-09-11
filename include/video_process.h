/******************************************************************************
 * Filename:    video_process.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESS_H
#define VIDEO_PROCESS_H

#include <opencv2/opencv.hpp>

#include "task.h"
#include "video_capture.h"

class VideoProcess {
   public:
    VideoProcess(VideoCapture& video_capture_);
    ~VideoProcess();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }

   private:
    const char* kWindowName = "Video";
    static void VideoProcessFunction(Task* task);
    Task task_;
    VideoCapture& video_capture_;
};

#endif  // VIDEO_PROCESS_H
