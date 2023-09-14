/******************************************************************************
 * Filename:    app.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "app.h"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

#include "colorspace_transformer.h"
#include "haar_cascade_classifier.h"
#include "logger.h"
#include "task.h"
#include "video_consumer.h"
#include "video_player.h"
#include "video_source.h"
#include "video_transformer.h"

App::App(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
         std::atomic<bool>& shutting_down, std::condition_variable& shutdown_cv)
    : task_(id, priority, period_ms, TaskFcn),
      shutting_down_(shutting_down),
      shutdown_cv_(shutdown_cv),
      video_input_(
          std::make_shared<VideoSourceFactory>(VideoSourceType::WEBCAM),
          TaskId::VIDEO_INPUT, TaskPriority::VIDEO_INPUT,
          TaskUpdatePeriodMs(33), shutting_down),
      video_processor_(video_input_,
                       std::make_shared<ColorspaceTransformerFactory>(),
                       TaskId::VIDEO_PROCESSING, TaskPriority::VIDEO_PROCESSING,
                       shutting_down),
      video_output_(video_processor_,
                    std::make_shared<VideoPlayerFactory>("Video Player"),
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
    std::vector<std::string> tokens;

    self->Help();
    while (!self->shutting_down_) {
        self->GetTokenizedInput(tokens);
        self->ParseTokens(tokens);
        self->Throttle();
    }
}

void App::GetTokenizedInput(std::vector<std::string>& tokens) {
    tokens.clear();
    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }
}

void App::ParseTokens(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return;
    }

    auto token = tokens.front();
    tokens.erase(tokens.begin());

    if (token == "q" || token == "quit") {
        Quit();
    } else if (token == "h" || token == "help") {
        Help();
    } else if (token == "start") {
        Start();
    } else if (token == "pause") {
        Pause();
    } else if (token == "stop") {
        Stop();
    } else if (token == "stats") {
        PrintStats();
    } else if (token == "i" || token == "input") {
        ParseInputTokens(tokens);
    } else if (token == "p" || token == "processing") {
        ParseProcessingTokens(tokens);
    } else {
        spdlog::warn(
            "Invalid command. Type 'help' to see a list of valid commands");
    }
}

void App::ParseInputTokens(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        Help("input");
        return;
    }

    auto token = tokens.front();
    tokens.erase(tokens.begin());

    if (token == "webcam") {
        SetSourceWebcam();
    } else if (token == "file") {
        if (tokens.empty()) {
            spdlog::error("You must provide a filename");
        }
        auto filename = tokens.front();
        SetSourceVideoFile(filename);

    } else {
        spdlog::error(
            "Invalid command. Type 'input' to see a list of the valid input "
            "commands");
    }
}

void App::ParseProcessingTokens(std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        Help("processing");
        return;
    }

    auto token = tokens.front();
    tokens.erase(tokens.begin());

    if (token == "bypass") {
        SetTransformerColorspace(Colorspace::BYPASS);
    } else if (token == "gray") {
        SetTransformerColorspace(Colorspace::BGR2GRAY);
    } else if (token == "hsv") {
        SetTransformerColorspace(Colorspace::BGR2HSV);
    } else if (token == "haar") {
        if (tokens.empty()) {
            Help("processing haar");
            return;
        }
        auto haar_processing_type = tokens.front();

        if (haar_processing_type == "eyes") {
            SetTrasformerHaarCascadeClassifier(HaarCascadeClassifierType::Eyes);
        } else if (haar_processing_type == "left_eye") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::LeftEye);
        } else if (haar_processing_type == "right_eye") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::RightEye);
        } else if (haar_processing_type == "eyes_w_glasses") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::EyesWithGlasses);
        } else if (haar_processing_type == "face") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::FrontalFace);
        } else if (haar_processing_type == "face_alt") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::FrontalFaceAlt);
        } else if (haar_processing_type == "face_alt2") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::FrontalFaceAlt2);
        } else if (haar_processing_type == "face_alt_tree") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::FrontalFaceAltTree);
        } else if (haar_processing_type == "face_profile") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::ProfileFace);
        } else if (haar_processing_type == "smile") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::Smile);
        } else if (haar_processing_type == "body") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::FullBody);
        } else if (haar_processing_type == "upper_body") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::UpperBody);
        } else if (haar_processing_type == "lower_body") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::LowerBody);
        } else if (haar_processing_type == "cat_face") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::CatFrontalFace);
        } else if (haar_processing_type == "cat_face_ext") {
            SetTrasformerHaarCascadeClassifier(
                HaarCascadeClassifierType::CatFrontalFaceExtended);
        } else {
            spdlog::error(
                "Invalid haar processing command type. Type 'processing haar' "
                "to see a list of the valid haar processing commands");
        }

    } else {
        spdlog::error(
            "Invalid command. Type 'processing' to see a list of the valid "
            "processing commands");
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
        "  ('input' or 'i')        : Set the input source of the sensory "
        "processing pipeline");
    spdlog::info(
        "  ('processing' or 'p')   : Set the processing strategy for the "
        "sensory processing pipeline");
    spdlog::info(
        "  ('start')               : Start sensory processing pipeline");
    spdlog::info(
        "  ('pause')               : Pause the sensory processing pipeline");
    spdlog::info(
        "  ('stop')                : Stop sensory processing pipeline");
    spdlog::info(
        "  ('stats')               : Print out statistics for the sensory "
        "processing pipeline");
}

void App::Help(const std::string help_type) {
    if (help_type == "input") {
        spdlog::info("Input Commands:");
        spdlog::info("  ('webcam')               : Set video source to webcam");
        spdlog::info("  ('file <filename.mp4>')  : Set video source to file");
    } else if (help_type == "processing") {
        spdlog::info("Processing Commands:");
        spdlog::info(
            "  ('bypass')               : Bypass the video processing");
        spdlog::info("  ('gray')                 : Convert the video to gray ");
        spdlog::info("  ('hsv')                  : Convert the video to hsv");
        spdlog::info(
            "  ('haar')                 : Object detection using a haar "
            "cascade classifier");
    } else if (help_type == "processing haar") {
        spdlog::info("Haar Cascade Classifier Commands:");
        spdlog::info("  ('eyes')                 : Draw a box around eyes");
        spdlog::info(
            "  ('left_eye')             : Draw boxes around left eyes");
        spdlog::info(
            "  ('right_eye')            : Draw boxes around right eyes");
        spdlog::info(
            "  ('eyes_w_glasses')       : Draw boxes around the eyes (when "
            "glasses could be on)");
        spdlog::info("  ('face')                 : Draw boxes around faces");
        spdlog::info(
            "  ('face_alt')             : Draw boxes around faces (alt)");
        spdlog::info(
            "  ('face_alt2')            : Draw boxes around faces (alt2)");
        spdlog::info(
            "  ('face_alt_tree')        : Draw boxes around faces (alt tree)");
        spdlog::info(
            "  ('face_profile')         : Draw boxes around the side of faces");
        spdlog::info("  ('smile')                : Draw boxes around smiles");
        spdlog::info(
            "  ('body')                 : Draw boxes around full bodies");
        spdlog::info(
            "  ('upper_body')           : Draw boxes around upper bodies");
        spdlog::info(
            "  ('lower_body')           : Draw boxes around lower bodies");
        spdlog::info(
            "  ('cat_face')             : Draw boxes around cat faces");
        spdlog::info(
            "  ('cat_face_ext')         : Draw boxes around cat faces "
            "(extended)");
    }

    else {
        spdlog::error("Invalid help type");
    }
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

void App::PrintStats() {
    std::string diagnostics_file = "diagnostics/diagnostics_out.txt";

    // Open the file
    std::ifstream file(diagnostics_file);

    // Check if the file is open
    if (!file.is_open()) {
        spdlog::error("Failed to open file: {}", diagnostics_file);
    }

    // Read and log each line of the file
    std::string line;
    while (std::getline(file, line)) {
        spdlog::info(line);
    }

    // Close the file
    file.close();
}

void App::SetSourceWebcam() {
    video_input_.ChangeSource(
        std::make_shared<VideoSourceFactory>(VideoSourceType::WEBCAM));
}

void App::SetSourceVideoFile(const std::string filename) {
    video_input_.ChangeSource(
        std::make_shared<VideoSourceFactory>(VideoSourceType::FILE, filename));
}

void App::SetTransformerColorspace(Colorspace colorspace) {
    video_processor_.ChangeTransformer(
        std::make_shared<ColorspaceTransformerFactory>(colorspace));
}

void App::SetTrasformerHaarCascadeClassifier(HaarCascadeClassifierType type) {
    video_processor_.ChangeTransformer(
        std::make_shared<HaarCascadeClassifierFactory>(type));
}

void App::Quit() {
    spdlog::info("Quiting application");
    shutting_down_ = true;
    shutdown_cv_.notify_all();
}