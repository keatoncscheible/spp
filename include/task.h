/******************************************************************************
 * Filename:    task.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef TASK_H
#define TASK_H

#include <pthread.h>

#include "time_defs.h"

// Forward declaration of Task class
class Task;

/***********************************************
Typedefs
***********************************************/
typedef void* (*TaskFunction)(Task*);

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
    TaskUpdatePeriodMs period_ms;

    void start();
    void join();

   private:
    TaskId id_;
    TaskPriority priority_;
    TaskFunction function_;
    pthread_t thread_;
    pthread_attr_t attr_;
    struct sched_param param_;
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
