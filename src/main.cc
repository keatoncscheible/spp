/******************************************************************************
 * Filename:    main.cpp
 * Description: Entry point of the application. Sets up signal handling,
 *              initializes tasks, and waits for them to complete before
 *              shutting down.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <mutex>

#include "app.h"
#include "error_handling.h"
#include "logger.h"
#include "task.h"

// Used to gracefully shutdown tasks
std::atomic<bool> shutting_down(false);
std::mutex shutdown_mutex;
std::condition_variable shutdown_cv;

// Initialize the logger
Logger &logger = Logger::instance();

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        shutting_down = true;
        shutdown_cv.notify_all();
    }
}

// Entry point of the application
int main(int argc, char *argv[]) {
    std::signal(SIGINT, SignalHandler);

    try {
        App app(TaskId::APP, TaskPriority::APP, TaskUpdatePeriodMs(100),
                shutting_down, shutdown_cv);
        app.Init();

        while (!shutting_down) {
            std::unique_lock<std::mutex> lock(shutdown_mutex);
            shutdown_cv.wait(lock);
        }

        app.Shutdown();

        return 0;

    } catch (const std::exception &e) {
        spdlog::error("An error occurred: {}", e.what());
        return 1;
    }
}
