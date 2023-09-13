/******************************************************************************
 * Filename:    video_file.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_FILE_H
#define VIDEO_FILE_H

#include <iostream>

#include "opencv2/core.hpp"
#include "video_source.h"

class VideoFile : public VideoSource {
   public:
    explicit VideoFile(const std::string& filename) : filename_(filename) {}
    void Open() override;
    void Close() override;
    void ReadFrame(cv::Mat& frame) override;

   private:
    std::string filename_;
};

class VideoFileFactory : public VideoSourceFactory {
   public:
    VideoFileFactory(const std::string& filename) : filename_(filename) {}

    std::shared_ptr<VideoSource> Create() override {
        return std::make_shared<VideoFile>(filename_);
    }

   private:
    std::string filename_;
};

#endif  // VIDEO_FILE_H
