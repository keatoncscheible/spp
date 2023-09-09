/******************************************************************************
 * Filename:    video_process.cpp
 * Description: Video processing task
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_process.h"

#include <atomic>
#include <iostream>

#include "task.h"

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
    while (!shutting_down) {
    }
    std::cout << "Shutting down video processing\n";
}