/******************************************************************************
 * Filename:    video_capture.cpp
 * Description: Video capture task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_capture.h"

#include <atomic>
#include <iostream>

extern std::atomic<bool> shutting_down;

/***********************************************
Functions
***********************************************/

/******************************************************************************
 * @brief Video capture task
 * @param arg TODO
 * @return TODO
 *****************************************************************************/
void* VideoCaptureTask(void* arg) {
    while (!shutting_down) {
    }
    std::cout << "Shutting down video capture\n";
    return nullptr;
}