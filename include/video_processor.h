/******************************************************************************
 * Filename:    video_processor.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <memory>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_task.h"
#include "video_transformer.h"

class VideoProcessor : public VideoTask {
   public:
    VideoProcessor(VideoTask& input,
                   std::shared_ptr<VideoTransformerFactory> transformer_factory,
                   TaskId id, TaskPriority priority,
                   TaskUpdatePeriodMs update_period,
                   std::atomic<bool>& shutting_down);
    ~VideoProcessor();
    void ChangeTransformer(
        std::shared_ptr<VideoTransformerFactory> new_transformer_factory);

   private:
    static void TaskFcn(Task* task);
    void GetInputFrame(cv::Mat& frame);
    void ProcessFrame(cv::Mat& frame);
    VideoTask& input_;
    std::shared_ptr<VideoTransformerFactory> transformer_factory_;
    std::shared_ptr<VideoTransformer> transformer_;
};

#endif  // VIDEO_PROCESSOR_H