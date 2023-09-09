/******************************************************************************
 * Filename:    task.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef TASK_H
#define TASK_H

#include <chrono>
#include <thread>

// Forward declaration of Task class
class Task;

/***********************************************
Aliases
***********************************************/
using TaskFunction = void (*)(Task*);

/***********************************************
Enums
***********************************************/
enum class TaskId {
    VIDEO_CAPTURE,
    VIDEO_PROCESS,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
    COUNT
};

enum class TaskPriority {
    VIDEO_CAPTURE = 1,
    VIDEO_PROCESS,
#ifdef DIAGNOSTICS_ENABLED
    DIAGNOSTICS,
#endif
};

enum class TaskUpdatePeriodMs {
    VIDEO_CAPTURE = 0,
    VIDEO_PROCESS = 0,
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

    void start();
    void join();

   private:
    TaskId id_;
    TaskPriority priority_;
    TaskFunction function_;
    std::thread thread_;
};

/***********************************************
Variables
***********************************************/
extern Task tasks[static_cast<int>(TaskId::COUNT)];

/***********************************************
Function Prototypes
***********************************************/
void task_init();
void task_shutdown();

#endif  // TASK_H
