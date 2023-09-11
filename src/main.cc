/******************************************************************************
 * Filename:    main.cpp
 * Description: Entry point of the application. Sets up signal handling,
 *              initializes tasks, and waits for them to complete before
 *              shutting down.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <unistd.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include "diagnostics.h"
#include "task.h"
#include "video_capture.h"
#include "video_process.h"

// Used to gracefully shutdown tasks
std::atomic<bool> shutting_down(false);

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\nClosing Application..." << std::endl;
        shutting_down = true;
    }
}

// Entry point of the application
int main(int argc, char* argv[]) {
    std::signal(SIGINT, SignalHandler);

    VideoCapture video_capture;
    VideoProcess video_process(video_capture);
    Diagnostics diagnostics(video_capture, video_process);

    video_capture.Start();
    video_process.Start();
    diagnostics.Start();

    while (!shutting_down) {
        constexpr int64_t kSleepDuration = 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(kSleepDuration));
    }

    diagnostics.Join();
    video_process.Join();
    video_capture.Join();

    return 0;
}
