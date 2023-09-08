#include "video_process.h"

#include <atomic>
#include <iostream>

extern std::atomic<bool> shutting_down;

/***********************************************
Functions
***********************************************/
void* task_video_process(void* arg) {
    while (!shutting_down) {
    }
    std::cout << "Shutting down video processing..." << std::endl;
    return nullptr;
}