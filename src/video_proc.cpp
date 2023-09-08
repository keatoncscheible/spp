#include "video_proc.h"

#include <atomic>
#include <iostream>

extern std::atomic<bool> shutting_down;

/***********************************************
Functions
***********************************************/
void* task_video_proc(void* arg) {
    while (!shutting_down) {
    }
    std::cout << "Shutting down video proc..." << std::endl;
    return nullptr;
}