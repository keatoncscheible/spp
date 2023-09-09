/******************************************************************************
 * Filename:    diagnostics.cpp
 * Description: Diagnostics task implementation. This file defines the behavior
 *              of the diagnostics task, responsible for generating diagnostics
 *              information and writing it to a file at regular intervals.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "diagnostics.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>

extern std::atomic<bool> shutting_down;

namespace fs = std::filesystem;

void DiagnosticsTask(Task* task) {
    auto period_ms = task->period_ms;
    std::mutex mutex;
    std::condition_variable cond;

    // Check if the diagnostics folder exists; if not, create it
    if (!fs::exists(DIAGNOSTICS_FOLDER)) {
        if (!fs::create_directory(DIAGNOSTICS_FOLDER)) {
            std::cerr << "Failed to create " << DIAGNOSTICS_FOLDER << std::endl;
        }
    }

    // Open a file for writing diagnostics information
    std::ofstream log_file(DIAGNOSTICS_FILE, std::ios::out | std::ios::trunc);

    int iter = 0;
    while (!shutting_down) {
        iter++;
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait_for(lock, period_ms);

        // Write diagnostics information to the log file
        log_file << "Diagnostics: " << iter << "\n";
        log_file.seekp(0);
    }

    // Close the log file and clean up resources
    log_file.close();

    try {
        // If the diagnostics folder exists, remove it and its contents
        if (std::filesystem::exists(DIAGNOSTICS_FOLDER)) {
            std::filesystem::remove_all(DIAGNOSTICS_FOLDER);
            std::cout << "Cleaning up diagnostics info\n";
        } else {
            std::cerr << "No diagnostics info found" << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
