/******************************************************************************
 * Filename:    diagnostics.cpp
 * Description: Diagnostics task implementation
 *              This file defines the behavior of the diagnostics task,
 *              responsible for generating diagnostics information.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "diagnostics.h"

#include <pthread.h>
#include <stdint.h>
#include <time.h>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "task.h"
#include "time_defs.h"

extern std::atomic<bool> shutting_down;

namespace fs = std::filesystem;

/***********************************************
Functions
***********************************************/

/******************************************************************************
 * @brief Helper function to add timespec values
 *
 * This function calculates the sum of two timespec values, considering
 * possible overflow in the nanosecond field.
 *
 * @param result Reference to the result timespec.
 * @param ts1    First timespec value.
 * @param ts2    Second timespec value to be added.
 *****************************************************************************/
static void TimespecAdd(struct timespec& result, const struct timespec& ts1,
                        const struct timespec& ts2) {
    result.tv_sec = ts1.tv_sec + ts2.tv_sec;
    result.tv_nsec = ts1.tv_nsec + ts2.tv_nsec;

    // Normalize the timespec (handle overflow)
    if (result.tv_nsec >= NS_PER_SEC) {
        result.tv_sec++;
        result.tv_nsec -= NS_PER_SEC;
    }
}

/******************************************************************************
 * @brief Diagnostics task function.
 *
 * This function is responsible for generating diagnostics information and
 * writing it to a file at regular intervals.
 *
 * @param arg Pointer to the Task object for the diagnostics task.
 * @return nullptr.
 *****************************************************************************/
void* task_diagnostics(void* arg) {
    Task* task = static_cast<Task*>(arg);
    uint64_t period_ms = static_cast<uint64_t>(task->period_ms);
    struct timespec delay;
    delay.tv_sec = period_ms / MS_PER_SEC;
    delay.tv_nsec = (period_ms - delay.tv_sec * MS_PER_SEC) * NS_PER_MS;

    // Initialize a mutex for synchronization
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    // Initialize a condition variable for timed waiting
    pthread_cond_t cond;
    pthread_cond_init(&cond, nullptr);

    // Check if the diagnostics folder exists; if not, create it
    if (!fs::exists(DIAGNOSTICS_FOLDER)) {
        if (!fs::create_directory(DIAGNOSTICS_FOLDER)) {
            std::cerr << "Failed to create " << DIAGNOSTICS_FOLDER << std::endl;
        }
    }

    // Open a file for writing diagnostics information
    std::ofstream log_file(DIAGNOSTICS_FILE, std::ios::out | std::ios::trunc);

    int iter = 0;
    while (!shutting_down) {
        iter++;
        pthread_mutex_lock(&mutex);

        // Calculate the absolute time for timedwait
        struct timespec abs_time;
        clock_gettime(CLOCK_REALTIME, &abs_time);
        TimespecAdd(abs_time, abs_time, delay);
        pthread_cond_timedwait(&cond, &mutex, &abs_time);

        // Write diagnostics information to the log file
        log_file << "Diagnostics: " << iter << "\n";
        log_file.seekp(0);

        pthread_mutex_unlock(&mutex);
    }

    // Close the log file and clean up resources
    log_file.close();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    try {
        // If the diagnostics folder exists, remove it and its contents
        if (std::filesystem::exists(DIAGNOSTICS_FOLDER)) {
            std::filesystem::remove_all(DIAGNOSTICS_FOLDER);
            std::cout << "Cleaning up diagnostics info\n";
        } else {
            std::cerr << "No diagnostics info found" << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return nullptr;
}
