#include "task.h"

#include <pthread.h>

#include "diagnostics.h"
#include "video_capture.h"
#include "video_process.h"

/***********************************************
Constants
***********************************************/
Task tasks[static_cast<int>(TaskId::COUNT)] = {
    {TaskId::VIDEO_CAPTURE, TaskPriority::VIDEO_CAPTURE, task_video_capture},
    {TaskId::VIDEO_PROC, TaskPriority::VIDEO_PROC, task_video_process},
#ifdef DIAGNOSTICS_ENABLED
    {TaskId::DIAGNOSTICS, TaskPriority::DIAGNOSTICS, task_diagnostics}
#endif
};

/***********************************************
Functions
***********************************************/
Task::Task(TaskId id, TaskPriority priority, TaskFunction function)
    : id_(id), priority_(priority), function_(function) {
    pthread_attr_init(&attr_);
    pthread_attr_setinheritsched(&attr_, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr_, SCHED_FIFO);
    param_.sched_priority = static_cast<int>(priority_);
    pthread_attr_setschedparam(&attr_, &param_);
}

void Task::start() { pthread_create(&thread_, &attr_, function_, this); }

void Task::join() {
    pthread_join(thread_, nullptr);
    pthread_attr_destroy(&attr_);
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