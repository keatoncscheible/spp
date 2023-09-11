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

/***********************************************
Enums
***********************************************/
enum class TaskId {
    VIDEO_CAPTURE,
    VIDEO_PROCESS,
    VIDEO_DISPLAY,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
    COUNT
};

enum class TaskPriority {
    VIDEO_CAPTURE = 1,
    VIDEO_PROCESS,
    VIDEO_DISPLAY,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
};

enum class TaskUpdatePeriodMs {
    VIDEO_CAPTURE = 0,
    VIDEO_PROCESS = 0,
    VIDEO_DISPLAY = 0,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS = 1000,
#endif
};

/***********************************************
Classes
***********************************************/
class Task {
   public:
    Task(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
         TaskFunction function);
    std::chrono::milliseconds period_ms;

    void Start();
    void Join();
    void* GetData();
    void SetData(void* data);

   private:
    TaskId id_;
    TaskPriority priority_;
    TaskFunction function_;
    void* data_;
    std::thread thread_;
};

#endif  // TASK_H
