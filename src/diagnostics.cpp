#include "diagnostics.h"

#include <pthread.h>
#include <time.h>

#include <fstream>
#include <iostream>

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

    std::ofstream log_file("diagnostics/diagnostics_out.txt");

    while (true) {
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

        log_file << "Diagnostics" << std::endl;
        log_file.flush();  // Flush the buffer to ensure data is written

        pthread_mutex_unlock(&mutex);
    }

    log_file.close();
    pthread_mutex_destroy(&mutex);  // Clean up mutex
    pthread_cond_destroy(&cond);    // Clean up condition variable

    return nullptr;
}
