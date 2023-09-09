/******************************************************************************
 * Filename:    task.cpp
 * Description: Task definitions for the application. This file defines task
 *              creation, management, and cleanup.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "task.h"

#include <pthread.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "diagnostics.h"
#include "video_capture.h"
#include "video_process.h"

/***********************************************
Constants
***********************************************/
Task tasks[static_cast<int>(TaskId::COUNT)] = {
    {
        TaskId::VIDEO_CAPTURE,
        TaskPriority::VIDEO_CAPTURE,
        TaskUpdatePeriodMs::VIDEO_CAPTURE,
        VideoCaptureTask,
    },
    {
        TaskId::VIDEO_PROCESS,
        TaskPriority::VIDEO_PROCESS,
        TaskUpdatePeriodMs::VIDEO_PROCESS,
        VideoProcessTask,
    },
#ifdef DIAGNOSTICS_ENABLED
    {
        TaskId::DIAGNOSTICS,
        TaskPriority::DIAGNOSTICS,
        TaskUpdatePeriodMs::DIAGNOSTICS,
        DiagnosticsTask,
    }
#endif
};

Task::Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
           TaskFunction function)
    : id_(id),
      priority_(priority),
      period_ms(static_cast<int>(period_ms)),
      function_(function) {}

void Task::start() {
    thread_ = std::thread(function_, this);
    sched_param sch_params;
    sch_params.sched_priority = static_cast<int>(priority_);
    if (pthread_setschedparam(thread_.native_handle(), SCHED_FIFO,
                              &sch_params) != 0) {
        std::cerr << "Failed to set thread priority: " << strerror(errno)
                  << std::endl;
    }
}

void Task::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

void task_init() {
    for (Task& task : tasks) {
        task.start();
    }
}

void task_shutdown() {
    for (Task& task : tasks) {
        task.join();
    }
}
