/******************************************************************************
 * Filename:    ui.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "ui.h"

#include <atomic>
#include <future>
#include <iostream>
#include <mutex>
#include <string>

#include "logger.h"

Ui::Ui(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
       std::atomic<bool>& shutting_down)
    : task_(id, priority, period_ms, TaskFcn), shutting_down_(shutting_down) {
    task_.SetData(this);
}

Ui::~Ui() {}

void Ui::TaskFcn(Task* task) {
    Ui* self = static_cast<Ui*>(task->GetData());
    std::string command;

    self->Help();
    while (!self->shutting_down_) {
        self->GetInput(command);
        self->ProcessInput(command);
        self->Throttle();
    }
}

void Ui::GetInput(std::string& input) { std::cin >> input; }

void Ui::ProcessInput(std::string& input) {
    if (input == "q" || input == "quit") {
        Quit();
    } else if (input == "h" || input == "help") {
        Help();
    } else {
        spdlog::warn(
            "{} is an invalid command. Type 'help' to see a list of valid "
            "command",
            input);
    }
}

void Ui::Throttle() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait_for(lock, task_.period_ms_);
}

void Ui::Help() {
    spdlog::info("Commands:");
    spdlog::info("  ('h' or 'help')    : Show all available commads");
    spdlog::info("  ('q' or 'quit')    : Quit the application");
}

void Ui::Quit() {
    spdlog::info("Quiting application");
    shutting_down_ = true;
}