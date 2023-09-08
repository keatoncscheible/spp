#include "video_capture.h"

#include <atomic>
#include <iostream>

extern std::atomic<bool> shutting_down;

/***********************************************
Functions
***********************************************/
void* task_video_capture(void* arg) {
    while (!shutting_down) {
    }
    std::cout << "Shutting down video capture..." << std::endl;
    return nullptr;
}