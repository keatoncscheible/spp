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

    std::ofstream logFile("diagnostics/diagnostics_log.txt");

    while (true) {
        pthread_mutex_lock(&mutex);

        // Calculate the absolute time for timedwait
        struct timespec absTime;
        clock_gettime(CLOCK_REALTIME, &absTime);
        absTime.tv_sec += delay.tv_sec;
        absTime.tv_nsec += delay.tv_nsec;
        if (absTime.tv_nsec >= 1000000000) {
            absTime.tv_sec++;
            absTime.tv_nsec -= 1000000000;
        }

        // Wait for the specified time or until signaled
        pthread_cond_timedwait(&cond, &mutex, &absTime);

        logFile << "Diagnostics" << std::endl;
        logFile.flush();  // Flush the buffer to ensure data is written

        pthread_mutex_unlock(&mutex);
    }

    logFile.close();
    pthread_mutex_destroy(&mutex);  // Clean up mutex
    pthread_cond_destroy(&cond);    // Clean up condition variable

    return nullptr;
}
