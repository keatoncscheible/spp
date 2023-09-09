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
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>

#include "diagnostics.h"
#include "task.h"

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

    task_init();

    while (!shutting_down) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    task_shutdown();

    return 0;
}
