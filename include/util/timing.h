/******************************************************************************
 * Filename:    task.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef TIMING_H
#define TTIMING_H

#include <chrono>

template <typename Func>
double TimeFunction(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();

    func();  // Call the function to be timed

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    return duration.count();  // Return the execution time in seconds
}

#endif  // TASK_H