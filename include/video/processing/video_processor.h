/******************************************************************************
 * Filename:    video_processor.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <memory>

#include "opencv2/core.hpp"
#include "statistics.h"
#include "task.h"
#include "video_task.h"
#include "video_transformer.h"

class VideoProcessor : public VideoTask {
   public:
    VideoProcessor(VideoTask& input,
                   std::shared_ptr<VideoTransformerFactory> transformer_factory,
                   TaskId id, TaskPriority priority,
                   std::atomic<bool>& shutting_down);
    ~VideoProcessor();
    void Start();
    void Stop();
    void ChangeTransformer(
        std::shared_ptr<VideoTransformerFactory> new_transformer_factory);
    StatisticsQueue<double> time_stats_{100};

   private:
    static void TaskFcn(Task* task);
    void GetInputFrame(cv::Mat& frame);
    void ProcessFrame(cv::Mat& frame);
    VideoTask& input_;
    std::shared_ptr<VideoTransformerFactory> transformer_factory_;
    std::shared_ptr<VideoTransformer> transformer_;
    bool running_;
};

#endif  // VIDEO_PROCESSOR_H