/******************************************************************************
 * Filename:    diagnostics.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>

#include "task.h"
#include "video_capture.h"
#include "video_process.h"

class Diagnostics {
   public:
    Diagnostics(VideoCapture& video_capture, VideoProcess& video_process);
    ~Diagnostics();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    std::mutex mutex;
    std::condition_variable cond;

   private:
    const std::string kDiagonsticsFolder = "diagnostics";
    const std::string kDiagonsticsFile =
        kDiagonsticsFolder + "/diagnostics_out.txt";
    static void DiagnosticsFunction(Task* task);
    void CreateDiagnosticsFolder();
    void OpenDiagnosticsFile();
    void CloseDiagnosticsFile();
    void RemoveDiagnosticsFolder();
    Task task_;
    VideoCapture& video_capture_;
    VideoProcess& video_process_;
    std::ofstream diagnostics_log_;
};

#endif  // DIAGNOSTICS_H
