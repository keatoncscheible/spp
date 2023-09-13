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
#include "logger.h"
#include "video_input.h"
#include "video_output.h"
#include "video_processor.h"

namespace fs = std::filesystem;

Diagnostics::Diagnostics(TaskId id, TaskPriority priority,
                         TaskUpdatePeriodMs period_ms, VideoInput& video_input,
                         VideoProcessor& video_processor,
                         VideoOutput& video_output,
                         std::atomic<bool>& shutting_down)
    : task_(id, priority, period_ms, TaskFcn),
      video_input_(video_input),
      video_processor_(video_processor),
      video_output_(video_output),
      shutting_down_(shutting_down) {
    task_.SetData(this);
    CreateDiagnosticsFolder();
    OpenDiagnosticsFile();
}

Diagnostics::~Diagnostics() {
    CloseDiagnosticsFile();
    RemoveDiagnosticsFolder();
    spdlog::info("Shutting down diagnostics");
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

void Diagnostics::ResetDiagnosticsLog() { diagnostics_log_.seekp(0); }

void Diagnostics::UpdateDiagnosticsLog() {
    ResetDiagnosticsLog();
    UpdateStatistics();
    constexpr int kPrecision = 2;
    diagnostics_log_ << "              Average     Minimum     Maximum     "
                        "Variance    Standard Deviation     \n";
    diagnostics_log_ << "Processing:   ";
    diagnostics_log_ << std::scientific << std::setprecision(kPrecision)
                     << video_processing_time_stats_.average << "    ";
    diagnostics_log_ << std::scientific << std::setprecision(kPrecision)
                     << video_processing_time_stats_.minimum << "    ";
    diagnostics_log_ << std::scientific << std::setprecision(kPrecision)
                     << video_processing_time_stats_.maximum << "    ";
    diagnostics_log_ << std::scientific << std::setprecision(kPrecision)
                     << video_processing_time_stats_.variance << "    ";
    diagnostics_log_ << std::scientific << std::setprecision(kPrecision)
                     << video_processing_time_stats_.standard_deviation
                     << "    ";
    diagnostics_log_ << "\n";
}

void Diagnostics::UpdateStatistics() {
    video_processing_time_stats_ = video_processor_.time_stats_.GetStatistics();
}

void Diagnostics::TaskFcn(Task* task) {
    Diagnostics* self = static_cast<Diagnostics*>(task->GetData());

    while (!self->shutting_down_) {
        std::unique_lock<std::mutex> lock(self->mutex);
        self->cond.wait_for(lock, task->period_ms_);
        self->UpdateDiagnosticsLog();
    }
}
