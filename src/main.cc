/******************************************************************************
 * Filename:    main.cpp
 * Description: Entry point of the application. Sets up signal handling,
 *              initializes tasks, and waits for them to complete before
 *              shutting down.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <logger.h>
#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <mutex>
#include <thread>

// #include "diagnostics.h"
#include "error_handling.h"
#include "grayscale_transformer.h"
#include "task.h"
#include "ui.h"
#include "video_consumer.h"
#include "video_file.h"
#include "video_input.h"
#include "video_output.h"
#include "video_player.h"
#include "video_processor.h"
#include "video_transformer.h"
#include "webcam.h"

// Used to gracefully shutdown tasks
std::atomic<bool> shutting_down(false);
std::mutex shutdown_mutex;
std::condition_variable shutdown_cv;

// Initialize the logger
Logger &logger = Logger::instance();

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        shutting_down = true;
        shutdown_cv.notify_all();
    }
}

// Entry point of the application
int main(int argc, char *argv[]) {
    std::signal(SIGINT, SignalHandler);

    try {
        std::shared_ptr<VideoSourceFactory> video_source_factory =
            std::make_shared<WebcamFactory>();

        std::shared_ptr<VideoTransformerFactory> video_transformer_factory =
            std::make_shared<GrayscaleTransformerFactory>();

        std::shared_ptr<VideoConsumerFactory> video_consumer_factory =
            std::make_shared<VideoPlayerFactory>("Video");

        Ui user_interface(TaskId::UI, TaskPriority::UI, TaskUpdatePeriodMs(100),
                          shutting_down);

        VideoInput video_input(video_source_factory, TaskId::VIDEO_INPUT,
                               TaskPriority::VIDEO_INPUT,
                               TaskUpdatePeriodMs(100), shutting_down);

        VideoProcessor video_processor(
            video_input, video_transformer_factory, TaskId::VIDEO_PROCESSING,
            TaskPriority::VIDEO_PROCESSING, shutting_down);

        VideoOutput video_output(
            video_processor, video_consumer_factory, TaskId::VIDEO_OUTPUT,
            TaskPriority::VIDEO_OUTPUT, TaskUpdatePeriodMs(100), shutting_down);

        user_interface.Init();
        video_input.Init();
        video_processor.Init();
        video_output.Init();

        // while (!shutting_down) {
        //     std::unique_lock<std::mutex> lock(shutdown_mutex);
        //     shutdown_cv.wait(lock);
        // }
        //     }
        // Wait for user input to switch video sources (e.g., press '1' for
        // Webcam, '2' for VideoFile)
        // char choice;
        // while (true) {
        //     spdlog::info(
        //         "Press '1' for Webcam, '2' for VideoFile, or 'q' to quit:
        //         ");

        //     std::cin >> choice;

        //     if (choice == '1') {
        //         // Switch to Webcam
        //         std::shared_ptr<VideoSourceFactory> webcam_factory =
        //             std::make_shared<WebcamFactory>();
        //         video_input.ChangeSource(webcam_factory);
        //     } else if (choice == '2') {
        //         // Switch to VideoFile
        //         std::shared_ptr<VideoSourceFactory> video_file_factory =
        //             std::make_shared<VideoFileFactory>("video.mp4");
        //         video_input.ChangeSource(video_file_factory);
        //     } else if (choice == 'q') {
        //         // Quit the program
        //         break;
        //     } else {
        //         spdlog::warn("Invalid choice. Please try again.");
        //     }
        // }

        // shutting_down = true;
        user_interface.Shutdown();
        video_output.Shutdown();
        video_processor.Shutdown();
        video_input.Shutdown();

    } catch (const std::exception &e) {
        spdlog::error("An error occurred: {}", e.what());
        return 1;
    }

    return 0;
}
