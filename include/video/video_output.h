/******************************************************************************
 * Filename:    video_output.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include <memory>

#include "opencv2/core.hpp"
#include "task.h"
#include "video_consumer.h"
#include "video_task.h"

class VideoOutput : public VideoTask {
   public:
    VideoOutput(VideoTask& input,
                std::shared_ptr<VideoConsumerFactory> consumer_factory,
                TaskId id, TaskPriority priority,
                TaskUpdatePeriodMs update_period,
                std::atomic<bool>& shutting_down);
    ~VideoOutput();
    void ChangeConsumer(
        std::shared_ptr<VideoConsumerFactory> new_consumer_factory);

   private:
    static void TaskFcn(Task* task);
    void GetInputFrame(cv::Mat& frame);
    void OutputFrame(cv::Mat& frame);
    VideoTask& input_;
    std::shared_ptr<VideoConsumerFactory> consumer_factory_;
    std::shared_ptr<VideoConsumer> consumer_;
};

#endif  // VIDEO_OUTPUT_H