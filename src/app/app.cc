/******************************************************************************
 * Filename:    app.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "app.h"

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <string>

#include "bypass_transformer.h"
#include "grayscale_transformer.h"
#include "hsv_transformer.h"
#include "logger.h"
#include "task.h"
#include "video_consumer.h"
#include "video_file.h"
#include "video_player.h"
#include "video_source.h"
#include "video_transformer.h"
#include "webcam.h"

App::App(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
         std::atomic<bool>& shutting_down, std::condition_variable& shutdown_cv)
    : task_(id, priority, period_ms, TaskFcn),
      shutting_down_(shutting_down),
      shutdown_cv_(shutdown_cv),
      webcam_factory_(std::make_shared<WebcamFactory>()),
      video_file_factory_(std::make_shared<VideoFileFactory>(
          "~/projects/spp/assets/videos/test.mp4")),
      bypass_transformer_factory_(std::make_shared<BypassTransformerFactory>()),
      grayscale_transformer_factory_(
          std::make_shared<GrayscaleTransformerFactory>()),
      hsv_transformer_factory_(std::make_shared<HsvTransformerFactory>()),
      video_player_factory_(
          std::make_shared<VideoPlayerFactory>("Video Player")),
      video_input_(webcam_factory_, TaskId::VIDEO_INPUT,
                   TaskPriority::VIDEO_INPUT, TaskUpdatePeriodMs(33),
                   shutting_down),
      video_processor_(video_input_, bypass_transformer_factory_,
                       TaskId::VIDEO_PROCESSING, TaskPriority::VIDEO_PROCESSING,
                       shutting_down),
      video_output_(video_processor_, video_player_factory_,
                    TaskId::VIDEO_OUTPUT, TaskPriority::VIDEO_OUTPUT,
                    TaskUpdatePeriodMs(33), shutting_down),
      diagnostics_(TaskId::DIAGNOSTICS, TaskPriority::DIAGNOSTICS,
                   TaskUpdatePeriodMs(1000), video_input_, video_processor_,
                   video_output_, shutting_down) {
    task_.SetData(this);
}

App::~App() {}

void App::Init() {
    task_.Start();
    video_input_.Init();
    video_processor_.Init();
    video_output_.Init();
    diagnostics_.Init();
}

void App::Shutdown() {
    diagnostics_.Shutdown();
    video_output_.Shutdown();
    video_processor_.Shutdown();
    video_input_.Shutdown();
    task_.Join();
}

void App::TaskFcn(Task* task) {
    App* self = static_cast<App*>(task->GetData());
    std::string command;

    self->Help();
    while (!self->shutting_down_) {
        self->GetInput(command);
        self->ProcessInput(command);
        self->Throttle();
    }
}

void App::GetInput(std::string& input) { std::getline(std::cin, input); }

void App::ProcessInput(std::string& input) {
    if (input == "q" || input == "quit") {
        Quit();
    } else if (input == "h" || input == "help") {
        Help();
    } else if (input == "start") {
        Start();
    } else if (input == "pause") {
        Pause();
    } else if (input == "stop") {
        Stop();
        // } else if (input == "source webcam") {
        //     SetSourceWebcam();
        // } else if (input == "source file") {
        //     SetSourceVideoFile();
    } else if (input == "transform bypass") {
        SetTransformerBypass();
    } else if (input == "transform gray") {
        SetTransformerGray();
    } else if (input == "transform hsv") {
        SetTransformerHsv();
    } else {
        spdlog::warn(
            "{} is an invalid command. Type 'help' to see a list of valid "
            "command",
            input);
    }
}

void App::Throttle() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait_for(lock, task_.period_ms_);
}

void App::Help() {
    spdlog::info("Commands:");
    spdlog::info("  ('help' or 'h')         : Show all available commads");
    spdlog::info("  ('quit' or 'q')         : Quit the application");
    spdlog::info(
        "  ('start')               : Start sensory processing pipeline");
    spdlog::info(
        "  ('pause')               : Pause the sensory processing pipeline");
    spdlog::info(
        "  ('stop')                : Stop sensory processing pipeline");
    // spdlog::info("  ('source webcam')       : Set video source to webcame");
    // spdlog::info("  ('source file')         : Set video source to file");
    spdlog::info("  ('transform bypass')    : Bypass the video transformer");
    spdlog::info(
        "  ('transform gray')      : Set video transformer to grayscale");
    spdlog::info("  ('transform hsv')       : Set video transformer to hsv");
}

void App::Start() {
    video_input_.Start();
    video_processor_.Start();
    video_output_.Start();
}

void App::Pause() {
    video_output_.Stop();
    video_processor_.Stop();
    video_input_.Stop();
}

void App::Stop() {
    video_output_.Stop();
    video_processor_.Stop();
    video_input_.Stop();
}

void App::SetSourceWebcam() { video_input_.ChangeSource(webcam_factory_); }

void App::SetSourceVideoFile() {
    video_input_.ChangeSource(video_file_factory_);
}

void App::SetTransformerBypass() {
    video_processor_.ChangeTransformer(bypass_transformer_factory_);
}

void App::SetTransformerGray() {
    video_processor_.ChangeTransformer(grayscale_transformer_factory_);
}

void App::SetTransformerHsv() {
    video_processor_.ChangeTransformer(hsv_transformer_factory_);
}

void App::Quit() {
    spdlog::info("Quiting application");
    shutting_down_ = true;
    shutdown_cv_.notify_all();
}