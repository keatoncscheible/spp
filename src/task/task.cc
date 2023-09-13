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

#include "logger.h"

Task::Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
           TaskFunction function)
    : id_(id),
      priority_(priority),
      period_ms_(period_ms),
      function_(function),
      data_(nullptr) {}

void Task::Start() {
    thread_ = std::thread(function_, this);
    sched_param sch_params;
    sch_params.sched_priority = static_cast<int>(priority_);
    if (pthread_setschedparam(thread_.native_handle(), SCHED_FIFO,
                              &sch_params) != 0) {
        spdlog::error("Failed to set thread priority: {}", strerror(errno));
    }
}

void Task::Join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

void* Task::GetData() { return data_; }
void Task::SetData(void* data) { data_ = data; }
