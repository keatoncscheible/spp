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

/***********************************************
Defines
***********************************************/
#define DIAGNOSTICS_FOLDER "diagnostics"
#define DIAGNOSTICS_FILE DIAGNOSTICS_FOLDER "/diagnostics_out.txt"

/***********************************************
Function Prototypes
***********************************************/
class Diagnostics {
   public:
    Diagnostics(VideoCapture& video_capture, VideoProcess& video_process);
    ~Diagnostics();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    static void DiagnosticsFunction(Task* task);
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    Task task_;
    VideoCapture& video_capture_;
    VideoProcess& video_process_;
    std::ofstream diagnostics_log_;
};

#endif  // DIAGNOSTICS_H
