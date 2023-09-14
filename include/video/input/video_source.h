/******************************************************************************
 * Filename:    video_source.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_SOURCE_H
#define VIDEO_SOURCE_H

#include <memory>
#include <string>

#include "logger.h"
#include "opencv2/opencv.hpp"

enum class VideoSourceType {
    WEBCAM,
    FILE,
};

class VideoSource {
   public:
    virtual void ReadFrame(cv::Mat& frame) = 0;
    virtual void Open() = 0;
    virtual void Close() = 0;
};

class Webcam : public VideoSource {
   public:
    void Open() override;
    void Close() override;
    void ReadFrame(cv::Mat& frame) override;

   private:
    cv::VideoCapture capture_;
};

using VideoSourceFilename = std::string;
class VideoFile : public VideoSource {
   public:
    explicit VideoFile(const VideoSourceFilename filename)
        : filename_(filename) {}
    void Open() override;
    void Close() override;
    void ReadFrame(cv::Mat& frame) override;

   private:
    VideoSourceFilename filename_;
    cv::VideoCapture capture_;
};

class VideoSourceFactory {
   public:
    VideoSourceFactory(VideoSourceType video_source_type)
        : video_source_type_(video_source_type), filename_("") {}
    VideoSourceFactory(VideoSourceType video_source_type,
                       VideoSourceFilename filename)
        : video_source_type_(video_source_type), filename_(filename) {}
    std::shared_ptr<VideoSource> Create() {
        if (video_source_type_ == VideoSourceType::WEBCAM) {
            return std::make_shared<Webcam>();
        } else if (video_source_type_ == VideoSourceType::FILE) {
            return std::make_shared<VideoFile>(filename_);
        } else {
            spdlog::error("Invalid Video Source Type");
            return nullptr;
        }
    }

   private:
    VideoSourceType video_source_type_;
    VideoSourceFilename filename_;
};

#endif  // VIDEO_SOURCE_H
