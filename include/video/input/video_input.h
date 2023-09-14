/******************************************************************************
 * Filename:    video_input.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_INPUT_H
#define VIDEO_INPUT_H

#include <memory>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_source.h"
#include "video_task.h"

class VideoInput : public VideoTask {
   public:
    VideoInput(std::shared_ptr<VideoSourceFactory> source_factory, TaskId id,
               TaskPriority priority, TaskUpdatePeriodMs update_period,
               std::atomic<bool>& shutting_down);
    ~VideoInput();
    void Init();
    void Start();
    void Stop();
    void Shutdown();
    void ChangeSource(std::shared_ptr<VideoSourceFactory> new_source_factory);

   private:
    void GetInputFrame(cv::Mat& frame);
    static void TaskFcn(Task* task);
    std::shared_ptr<VideoSourceFactory> source_factory_;
    std::shared_ptr<VideoSource> source_;
    bool running_;
};

#endif  // VIDEO_INPUT_H