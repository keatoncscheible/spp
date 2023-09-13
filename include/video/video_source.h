/******************************************************************************
 * Filename:    video_source.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_SOURCE_H
#define VIDEO_SOURCE_H

#include <memory>

#include "opencv2/core.hpp"

class VideoSource {
   public:
    virtual void ReadFrame(cv::Mat& frame) = 0;
    virtual void Open() = 0;
    virtual void Close() = 0;
};

class VideoSourceFactory {
   public:
    virtual std::shared_ptr<VideoSource> Create() = 0;
};

#endif  // VIDEO_SOURCE_H
