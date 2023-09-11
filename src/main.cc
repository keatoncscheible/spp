/******************************************************************************
 * Filename:    main.cpp
 * Description: Entry point of the application. Sets up signal handling,
 *              initializes tasks, and waits for them to complete before
 *              shutting down.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <unistd.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include "diagnostics.h"
#include "error_handling.h"
#include "task.h"
#include "video_capture.h"
#include "video_display.h"
#include "video_processing.h"

// Used to gracefully shutdown tasks
std::atomic<bool> shutting_down(false);

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        std::cout << "\nClosing Application..." << std::endl;
        shutting_down = true;
    }
}

// Entry point of the application
int main(int argc, char *argv[]) {
    std::signal(SIGINT, SignalHandler);

    try {
        VideoCapture video_capture;
        VideoProcessing video_processing(video_capture);
        VideoDisplay video_display(video_processing);
        Diagnostics diagnostics(video_capture, video_processing);

        video_capture.Start();
        video_processing.Start();
        video_display.Start();
        diagnostics.Start();

        while (!shutting_down) {
            constexpr int64_t kSleepDuration = 100;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(kSleepDuration));
        }

        diagnostics.Join();
        video_display.Join();
        video_processing.Join();
        video_capture.Join();

    } catch (const VideoCaptureException &vce) {
        std::cerr << "Video Capture Error: " << vce.what() << std::endl;
    } catch (const VideoProcessingException &vpe) {
        std::cerr << "Video Processinging Error: " << vpe.what() << std::endl;
    } catch (const VideoDisplayException &vde) {
        std::cerr << "Video Display Error: " << vde.what() << std::endl;
    } catch (const DiagnosticsException &de) {
        std::cerr << "Diagnostics Error: " << de.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}
