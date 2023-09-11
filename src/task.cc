/******************************************************************************
 * Filename:    task.cpp
 * Description: Task definitions for the application. This file defines task
 *              creation, management, and cleanup.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "task.h"

#include <pthread.h>

#include <array>
#include <cstring>
#include <iostream>
#include <thread>

#include "diagnostics.h"
#include "video_capture.h"
#include "video_process.h"

Task::Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
           TaskFunction function)
    : id_(id),
      priority_(priority),
      period_ms(static_cast<int>(period_ms)),
      function_(function),
      data_(nullptr) {}

void Task::Start() {
    thread_ = std::thread(function_, this);
    sched_param sch_params;
    sch_params.sched_priority = static_cast<int>(priority_);
    if (pthread_setschedparam(thread_.native_handle(), SCHED_FIFO,
                              &sch_params) != 0) {
        std::cerr << "Failed to set thread priority: " << strerror(errno)
                  << std::endl;
    }
}

void Task::Join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

void* Task::GetData() { return data_; }
void Task::SetData(void* data) { data_ = data; }