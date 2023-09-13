/******************************************************************************
 * Filename:    diagnostics.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>

#include "statistics.h"
#include "task.h"
#include "video_input.h"
#include "video_output.h"
#include "video_processor.h"

class Diagnostics {
   public:
    Diagnostics(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
                VideoInput& video_input, VideoProcessor& video_processor,
                VideoOutput& video_output, std::atomic<bool>& shutting_down);
    ~Diagnostics();
    void Init() { task_.Start(); }
    void Shutdown() { task_.Join(); }
    std::mutex mutex;
    std::condition_variable cond;

   private:
    const std::string kDiagonsticsFolder = "diagnostics";
    const std::string kDiagonsticsFile =
        kDiagonsticsFolder + "/diagnostics_out.txt";
    static void TaskFcn(Task* task);
    void CreateDiagnosticsFolder();
    void OpenDiagnosticsFile();
    void CloseDiagnosticsFile();
    void RemoveDiagnosticsFolder();
    void ResetDiagnosticsLog();
    void UpdateDiagnosticsLog();
    void UpdateStatistics();
    Task task_;
    VideoInput& video_input_;
    VideoProcessor& video_processor_;
    VideoOutput& video_output_;
    std::ofstream diagnostics_log_;
    Statistics<double> video_processing_time_stats_{0, 0, 0, 0, 0};
    std::atomic<bool>& shutting_down_;
};

#endif  // DIAGNOSTICS_H
