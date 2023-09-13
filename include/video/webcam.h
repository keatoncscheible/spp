/******************************************************************************
 * Filename:    webcam.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef WEBCAM_H
#define WEBCAM_H

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

#include "video_source.h"

class Webcam : public VideoSource {
   public:
    void Open() override;
    void Close() override;
    void ReadFrame(cv::Mat& frame) override;

   private:
    cv::VideoCapture capture_;
};

class WebcamFactory : public VideoSourceFactory {
   public:
    std::shared_ptr<VideoSource> Create() override {
        return std::make_shared<Webcam>();
    }
};

#endif  // WEBCAM_H
