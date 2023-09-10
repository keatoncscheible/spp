/******************************************************************************
 * Filename:    video_process.cpp
 * Description: Video processing task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_process.h"

#include <atomic>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "video_capture.h"

extern std::atomic<bool> shutting_down;

/***********************************************
Functions
***********************************************/

/******************************************************************************
 * @brief Video processing task
 * @param arg
 * @return TODO
 *****************************************************************************/
void VideoProcessTask(Task* task) {
    cv::Mat frame;
    int frame_cnt = 0;
    while (!shutting_down) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        buffer_cond.wait(lock);
        frame = *GetCurrentFrame();
        lock.unlock();
        cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
        cv::imshow("Video", frame);
        cv::waitKey(1);
    }
    cv::destroyWindow("Video");

    std::cout << "Shutting down video processing\n";
}
