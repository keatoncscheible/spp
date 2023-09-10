/******************************************************************************
 * Filename:    video_capture.cpp
 * Description: Video capture task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_capture.h"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>

extern std::atomic<bool> shutting_down;

// Double buffer
cv::Mat buffer1, buffer2;

// Current and next buffer pointers
cv::Mat* current_buffer = &buffer1;
cv::Mat* next_buffer = &buffer2;

// Synchronization mechanisms
std::mutex buffer_mutex;
std::condition_variable buffer_cond;

/***********************************************
Functions
***********************************************/

/******************************************************************************
 * @brief Video capture task
 * @param arg TODO
 * @return TODO
 *****************************************************************************/

void VideoCaptureTask(Task* task) {
    cv::VideoCapture capture(0);  // open the default camera (webcam)

    if (!capture.isOpened()) {
        std::cerr << "Error: Cannot open webcam!" << std::endl;
        return;
    }

    while (!shutting_down) {
        capture >> *next_buffer;

        if (next_buffer->empty()) {
            std::cerr << "Failed to capture frame." << std::endl;
            continue;
        }
        std::unique_lock<std::mutex> lock(buffer_mutex);
        std::swap(current_buffer, next_buffer);
        buffer_cond.notify_one();
    }

    // Release the video capture object
    capture.release();

    std::cout << "Shutting down video capture\n";
}

cv::Mat* GetCurrentFrame() { return current_buffer; }