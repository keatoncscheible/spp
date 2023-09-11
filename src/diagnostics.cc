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

#include "error_handling.h"
#include "video_capture.h"
#include "video_processing.h"

extern std::atomic<bool> shutting_down;

namespace fs = std::filesystem;

Diagnostics::Diagnostics(VideoCapture& video_capture,
                         VideoProcessing& video_processing)
    : video_capture_(video_capture),
      video_processing_(video_processing),
      task_(TaskId::DIAGNOSTICS, TaskPriority::DIAGNOSTICS,
            TaskUpdatePeriodMs::DIAGNOSTICS, DiagnosticsFunction) {
    task_.SetData(this);
    CreateDiagnosticsFolder();
    OpenDiagnosticsFile();
}

Diagnostics::~Diagnostics() {
    CloseDiagnosticsFile();
    RemoveDiagnosticsFolder();
    std::cout << "Shutting down diagnostics\n";
}

void Diagnostics::CreateDiagnosticsFolder() {
    if (!fs::exists(kDiagonsticsFolder)) {
        if (!fs::create_directory(kDiagonsticsFolder)) {
            throw DiagnosticsException("Error creating diagnostics folder");
        }
    }
}

void Diagnostics::OpenDiagnosticsFile() {
    diagnostics_log_.open(kDiagonsticsFile, std::ios::out);
    if (!diagnostics_log_.is_open()) {
        throw DiagnosticsException("Error creating diagnostics file");
    }
}

void Diagnostics::CloseDiagnosticsFile() {
    diagnostics_log_.close();
    if (diagnostics_log_.fail()) {
        throw DiagnosticsException("Error closing diagnostics file");
    }
}
void Diagnostics::RemoveDiagnosticsFolder() {
    try {
        if (std::filesystem::exists(kDiagonsticsFolder)) {
            std::filesystem::remove_all(kDiagonsticsFolder);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::string error_message =
            "Error removing diagnostics folder: " + std::string(e.what());
        throw DiagnosticsException(error_message);
    }
}

void Diagnostics::DiagnosticsFunction(Task* task) {
    Diagnostics* self = static_cast<Diagnostics*>(task->GetData());

    int iter = 0;
    while (!shutting_down) {
        iter++;
        std::unique_lock<std::mutex> lock(self->mutex);
        self->cond.wait_for(lock, task->period_ms);

        // Write diagnostics information to the log file
        self->diagnostics_log_ << "Diagnostics: " << iter << "\n";
        self->diagnostics_log_.seekp(0);
    }
}
