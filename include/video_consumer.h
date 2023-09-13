/******************************************************************************
 * Filename:    video_consumer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_CONSUMER_H
#define VIDEO_CONSUMER_H

#include <memory>

#include "opencv2/core.hpp"

class VideoConsumer {
   public:
    virtual void Consume(const cv::Mat& frame) = 0;
};

class VideoConsumerFactory {
   public:
    virtual std::shared_ptr<VideoConsumer> Create() = 0;
};

#endif  // VIDEO_CONSUMER_H
