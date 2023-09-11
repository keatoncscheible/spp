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

Diagnostics::Diagnostics(VideoCapture& video_capture,
                         VideoProcess& video_process)
    : video_capture_(video_capture),
      video_process_(video_process),
      task_(TaskId::DIAGNOSTICS, TaskPriority::DIAGNOSTICS,
            TaskUpdatePeriodMs::DIAGNOSTICS, DiagnosticsFunction) {
    task_.SetData(this);

    // Check if the diagnostics folder exists; if not, create it
    if (!fs::exists(DIAGNOSTICS_FOLDER)) {
        if (!fs::create_directory(DIAGNOSTICS_FOLDER)) {
            std::cerr << "Failed to create directory: " << DIAGNOSTICS_FOLDER
                      << ". Error: " << strerror(errno) << std::endl;
            return;  // If we can't create the directory, don't proceed further.
        }
    }

    // Open a file for writing diagnostics information
    diagnostics_log_.open(DIAGNOSTICS_FILE, std::ios::out | std::ios::trunc);
    if (!diagnostics_log_.is_open()) {
        std::cerr << "Failed to open file: " << DIAGNOSTICS_FILE
                  << ". Error: " << strerror(errno) << std::endl;
    }
}

Diagnostics::~Diagnostics() {
    diagnostics_log_.close();
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

void Diagnostics::DiagnosticsFunction(Task* task) {
    Diagnostics* self = static_cast<Diagnostics*>(task->GetData());

    int iter = 0;
    while (!shutting_down) {
        iter++;
        std::unique_lock<std::mutex> lock(self->mutex_);
        self->cond_.wait_for(lock, task->period_ms);

        // Write diagnostics information to the log file
        self->diagnostics_log_ << "Diagnostics: " << iter << "\n";
        self->diagnostics_log_.seekp(0);
    }
}
