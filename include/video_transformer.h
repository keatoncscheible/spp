/******************************************************************************
 * Filename:    video_transformer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_TRANSFORMER_H
#define VIDEO_TRANSFORMER_H

#include <memory>

#include "opencv2/core.hpp"
#include "video_source.h"

class VideoTransformer : public VideoSource {
   public:
    void Open() override{};
    void Close() override{};
    void ReadFrame(cv::Mat& frame) override{};
    virtual void Transform(cv::Mat& frame) = 0;
};

class VideoTransformerFactory {
   public:
    virtual std::shared_ptr<VideoTransformer> Create() = 0;
};

#endif  // VIDEO_TRANSFORMER_H
