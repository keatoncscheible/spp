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

#include "diagnostics.h"
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
    void GetInput(std::string& input);
    void ProcessInput(std::string& input);
    void Throttle();
    void Help();
    void Start();
    void Pause();
    void Stop();
    void PrintStats();
    void SetSourceWebcam();
    void SetSourceVideoFile();
    void SetTransformerBypass();
    void SetTransformerGray();
    void SetTransformerHsv();
    void Quit();
    Task task_;
    std::atomic<bool>& shutting_down_;
    std::condition_variable& shutdown_cv_;
    std::shared_ptr<VideoSourceFactory> webcam_factory_;
    std::shared_ptr<VideoSourceFactory> video_file_factory_;
    std::shared_ptr<VideoTransformerFactory> bypass_transformer_factory_;
    std::shared_ptr<VideoTransformerFactory> grayscale_transformer_factory_;
    std::shared_ptr<VideoTransformerFactory> hsv_transformer_factory_;
    std::shared_ptr<VideoConsumerFactory> video_player_factory_;
    VideoInput video_input_;
    VideoProcessor video_processor_;
    VideoOutput video_output_;
    Diagnostics diagnostics_;
};

#endif  // APP_H
