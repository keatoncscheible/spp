#include "diagnostics.h"

#include <pthread.h>
#include <time.h>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>

extern std::atomic<bool> shutting_down;

namespace fs = std::filesystem;

/***********************************************
Functions
***********************************************/
void* task_diagnostics(void* arg) {
    struct timespec delay;
    delay.tv_sec = 1;
    delay.tv_nsec = 0;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);  // Initialize mutex

    pthread_cond_t cond;
    pthread_cond_init(&cond, nullptr);  // Initialize condition variable

    // Check if the directory exists
    if (!fs::exists(DIAGNOSTICS_FOLDER)) {
        // If it doesn't exist, create it
        if (!fs::create_directory(DIAGNOSTICS_FOLDER)) {
            std::cerr << "Failed to create " << DIAGNOSTICS_FOLDER << std::endl;
        }
    }

    std::ofstream log_file(DIAGNOSTICS_FILE, std::ios::out | std::ios::trunc);

    int iter = 0;
    while (!shutting_down) {
        iter++;
        pthread_mutex_lock(&mutex);

        // Calculate the absolute time for timedwait
        struct timespec abs_time;
        clock_gettime(CLOCK_REALTIME, &abs_time);
        abs_time.tv_sec += delay.tv_sec;
        abs_time.tv_nsec += delay.tv_nsec;
        if (abs_time.tv_nsec >= 1000000000) {
            abs_time.tv_sec++;
            abs_time.tv_nsec -= 1000000000;
        }

        // Wait for the specified time or until signaled
        pthread_cond_timedwait(&cond, &mutex, &abs_time);

        log_file << "Diagnostics: " << iter << std::endl;
        log_file.flush();
        log_file.seekp(0);

        pthread_mutex_unlock(&mutex);
    }

    log_file.close();
    pthread_mutex_destroy(&mutex);  // Clean up mutex
    pthread_cond_destroy(&cond);    // Clean up condition variable

    return nullptr;
}
