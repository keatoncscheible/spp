/******************************************************************************
 * Filename:    task.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef TASK_H
#define TASK_H

#include <chrono>
#include <functional>
#include <thread>

// Forward declaration of Task class
class Task;

/***********************************************
Aliases
***********************************************/
using TaskFunction = std::function<void(Task*)>;
using TaskUpdatePeriodMs = std::chrono::milliseconds;

/***********************************************
Enums
***********************************************/
enum class TaskId {
    APP,
    VIDEO_INPUT,
    VIDEO_PROCESSING,
    VIDEO_OUTPUT,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
    COUNT
};

enum class TaskPriority {
    APP = 1,
    VIDEO_INPUT,
    VIDEO_PROCESSING,
    VIDEO_OUTPUT,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
    
};

/***********************************************
Classes
***********************************************/
class Task {
   public:
    Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
         TaskFunction function);
    void Start();
    void Join();
    void* GetData();
    void SetData(void* data);
    TaskUpdatePeriodMs period_ms_;

   private:
    TaskId id_;
    TaskPriority priority_;
    TaskFunction function_;
    void* data_;
    std::thread thread_;
};

#endif  // TASK_H
