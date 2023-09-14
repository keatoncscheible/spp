/******************************************************************************
 * Filename:    app.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef APP_H
#define APP_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

#include "colorspace_transformer.h"
#include "diagnostics.h"
#include "haar_cascade_classifier.h"
#include "task.h"
#include "video_consumer.h"
#include "video_input.h"
#include "video_output.h"
#include "video_processor.h"
#include "video_source.h"
#include "video_transformer.h"

class App {
   public:
    App(TaskId id, TaskPriority priority, TaskUpdatePeriodMs period_ms,
        std::atomic<bool>& shutting_down, std::condition_variable& shutdown_cv);
    ~App();
    void Init();
    void Shutdown();
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void TaskFcn(Task* task);
    void GetTokenizedInput(std::vector<std::string>& tokens);
    void ParseTokens(std::vector<std::string>& tokens);
    void ParseInputTokens(std::vector<std::string>& tokens);
    void ParseProcessingTokens(std::vector<std::string>& tokens);
    void Throttle();
    void Help();
    void Help(const std::string help_type);
    void Start();
    void Pause();
    void Stop();
    void PrintStats();
    void SetSourceWebcam();
    void SetSourceVideoFile(const std::string filename);
    void SetTransformerColorspace(Colorspace colorspace);
    void SetTrasformerHaarCascadeClassifier(HaarCascadeClassifierType type);
    void Quit();
    Task task_;
    std::atomic<bool>& shutting_down_;
    std::condition_variable& shutdown_cv_;
    VideoInput video_input_;
    VideoProcessor video_processor_;
    VideoOutput video_output_;
    Diagnostics diagnostics_;
};

#endif  // APP_H
