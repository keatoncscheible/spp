/******************************************************************************
 * Filename:    video_capture.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <condition_variable>
#include <opencv2/opencv.hpp>

#include "task.h"

/***********************************************
Function Prototypes
***********************************************/
void VideoCaptureTask(Task* task);
cv::Mat* GetCurrentFrame();

extern std::mutex buffer_mutex;
extern std::condition_variable buffer_cond;
extern cv::Mat* current_buffer;

#endif  // VIDEO_CAPTURE_H
