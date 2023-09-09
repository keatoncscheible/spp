/******************************************************************************
 * Filename:    task.cpp
 * Description: Task definitions for the application. This file defines task
 *              creation, management, and cleanup.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "task.h"

#include <pthread.h>

#include "diagnostics.h"
#include "time_defs.h"
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

/***********************************************
Functions
***********************************************/

/******************************************************************************
 * @brief Constructor for Task class.
 *
 * This constructor initializes a Task object with the provided parameters,
 * sets up the thread attributes, and configures the scheduling policy.
 *
 * @param id The ID of the task.
 * @param priority The priority of the task.
 * @param function The function to execute as the task.
 *****************************************************************************/
Task::Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
           TaskFunction function)
    : id_(id), priority_(priority), period_ms(period_ms), function_(function) {
    pthread_attr_init(&attr_);
    pthread_attr_setinheritsched(&attr_, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr_, SCHED_FIFO);
    param_.sched_priority = static_cast<int>(priority_);
    pthread_attr_setschedparam(&attr_, &param_);
}

/******************************************************************************
 * @brief Starts the task by creating a new thread.
 *
 * This function creates a new thread and assigns it the task's function.
 *****************************************************************************/
void Task::start() {
    pthread_create(&thread_, &attr_,
                   reinterpret_cast<void* (*)(void*)>(function_), this);
}

/******************************************************************************
 * @brief Waits for the task to complete and cleans up resources.
 *
 * This function waits for the task's thread to complete execution and then
 * cleans up the thread attributes.
 *****************************************************************************/
void Task::join() {
    pthread_join(thread_, nullptr);
    pthread_attr_destroy(&attr_);
}

/******************************************************************************
 * @brief Initializes and starts all tasks.
 *
 * This function initializes and starts all the tasks in the global 'tasks'
 * array. It is typically called at the beginning of the program.
 *****************************************************************************/
void task_init() {
    for (Task& task : tasks) {
        task.start();
    }
}

/******************************************************************************
 * @brief Shuts down all tasks.
 *
 * This function waits for all tasks to complete and cleans up any resources
 * associated with the tasks.
 *****************************************************************************/
void task_shutdown() {
    for (Task& task : tasks) {
        task.join();
    }
}
