/******************************************************************************
 * Filename:    ui.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef UI_H
#define UI_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

#include "task.h"

class Ui {
   public:
    Ui(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
       std::atomic<bool>& shutting_down);
    ~Ui();
    void Init() { task_.Start(); }
    void Shutdown() { task_.Join(); }
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void TaskFcn(Task* task);
    void GetInput(std::string& input);
    void ProcessInput(std::string& input);
    void Throttle();
    void Help();
    void Quit();
    Task task_;
    std::atomic<bool>& shutting_down_;
};

#endif  // UI_H
