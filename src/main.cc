/******************************************************************************
 * Filename:    main.cpp
 * Description: Entry point of the application. Sets up signal handling,
 *              initializes tasks, and waits for them to complete before
 *              shutting down.
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <mutex>
#include <thread>

// #include "diagnostics.h"
#include "error_handling.h"
#include "task.h"
// #include "video_capture.h"
// #include "video_display.h"
#include "video_file.h"
#include "video_input.h"
#include "video_player.h"
#include "video_transformer.h"
// #include "video_processing.h"
#include "grayscale_transformer.h"
#include "video_consumer.h"
#include "video_output.h"
#include "video_processor.h"
#include "webcam.h"

// Used to gracefully shutdown tasks
std::atomic<bool> shutting_down(false);
std::condition_variable shutdown_cv;
std::mutex shutdown_mutex;
std::condition_variable video_input_cv;
std::mutex video_input_mutex;
std::condition_variable process_cv;
std::mutex process_mutex;
std::condition_variable display_cv;
std::mutex display_mutex;

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        shutting_down = true;
        shutdown_cv.notify_all();
        process_cv.notify_all();
        video_input_cv.notify_all();
        display_cv.notify_all();
    }
}

// Entry point of the application
int main(int argc, char *argv[]) {
    std::signal(SIGINT, SignalHandler);

    // Create a VideoInput instance with an initial video source (e.g., Webcam)
    std::shared_ptr<VideoSourceFactory> video_source_factory =
        std::make_shared<WebcamFactory>();

    std::shared_ptr<VideoTransformerFactory> video_transformer_factory =
        std::make_shared<GrayscaleTransformerFactory>();

    std::shared_ptr<VideoConsumerFactory> video_consumer_factory =
        std::make_shared<VideoPlayerFactory>("Video");

    VideoInput video_input(video_source_factory, TaskId::VIDEO_INPUT,
                           TaskPriority::VIDEO_INPUT,
                           TaskUpdatePeriodMs::VIDEO_INPUT, shutting_down);

    VideoProcessor video_processor(
        video_input, video_transformer_factory, TaskId::VIDEO_PROCESSING,
        TaskPriority::VIDEO_PROCESSING, TaskUpdatePeriodMs::VIDEO_PROCESSING,
        shutting_down);

    VideoOutput video_output(video_processor, video_consumer_factory,
                             TaskId::VIDEO_DISPLAY, TaskPriority::VIDEO_DISPLAY,
                             TaskUpdatePeriodMs::VIDEO_DISPLAY, shutting_down);

    // Start the initial video source
    video_input.Init();
    video_processor.Init();
    video_output.Init();

    // Wait for user input to switch video sources (e.g., press '1' for Webcam,
    // '2' for VideoFile)
    char choice;
    while (true) {
        std::cout
            << "Press '1' for Webcam, '2' for VideoFile, or 'q' to quit: ";
        std::cin >> choice;

        if (choice == '1') {
            // Switch to Webcam
            std::shared_ptr<VideoSourceFactory> webcam_factory =
                std::make_shared<WebcamFactory>();
            video_input.ChangeSource(webcam_factory);
        } else if (choice == '2') {
            // Switch to VideoFile
            std::shared_ptr<VideoSourceFactory> video_file_factory =
                std::make_shared<VideoFileFactory>("video.mp4");
            video_input.ChangeSource(video_file_factory);
        } else if (choice == 'q') {
            // Quit the program
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    // Stop the video source before exiting
    video_output.Shutdown();
    video_processor.Shutdown();
    video_input.Shutdown();

    return 0;

    // try {
    //     VideoCapture video_capture;
    //     VideoProcessing video_processing(video_capture);
    //     VideoDisplay video_display(video_processing);
    //     Diagnostics diagnostics(video_capture, video_processing);

    //     video_capture.Start();
    //     video_processing.Start();
    //     video_display.Start();
    //     diagnostics.Start();

    //     while (!shutting_down) {
    //         std::unique_lock<std::mutex> lock(shutdown_mutex);
    //         shutdown_cv.wait(lock);
    //     }

    //     diagnostics.Join();
    //     video_display.Join();
    //     video_processing.Join();
    //     video_capture.Join();

    // } catch (const VideoCaptureException &vce) {
    //     std::cerr << "Video Capture Error: " << vce.what() << std::endl;
    // } catch (const VideoProcessingException &vpe) {
    //     std::cerr << "Video Processinging Error: " << vpe.what() <<
    //     std::endl;
    // } catch (const VideoDisplayException &vde) {
    //     std::cerr << "Video Display Error: " << vde.what() << std::endl;
    // } catch (const DiagnosticsException &de) {
    //     std::cerr << "Diagnostics Error: " << de.what() << std::endl;
    // } catch (const std::exception &e) {
    //     std::cerr << "An error occurred: " << e.what() << std::endl;
    // }

    // return 0;
}
