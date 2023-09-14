/******************************************************************************
 * Filename:    video_source.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_source.h"

#include <logger.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

void VideoFile::Open() {
    spdlog::info("Opening Video File: {}", filename_);
    capture_.open(filename_);
    if (!capture_.isOpened()) {
        spdlog::error("Error opening video file: {}", filename_);
        // Handle the error (e.g., throw an exception or return an error
        // code)
    }
}

void VideoFile::Close() {
    spdlog::info("Closing Video File: {}", filename_);
    capture_.release();
}

void VideoFile::ReadFrame(cv::Mat& frame) {
    if (!capture_.read(frame)) {
        spdlog::error("Error reading frame from Video File: {}", filename_);
        // Handle the error (e.g., return a flag to indicate end of video)
    }
}

void Webcam::Open() {
    spdlog::debug("Opening webcam");
    if (capture_.isOpened()) {
        spdlog::warn("Webcam already opened");
        return;
    }
    capture_.open(0);
    if (!capture_.isOpened()) {
        spdlog::error("Problem opening webcam");
    }
}

void Webcam::Close() {
    spdlog::debug("Closing webcam");
    try {
        if (capture_.isOpened()) {
            capture_.release();
        }
    } catch (const std::exception& e) {
        spdlog::error("Error stopping video capture: {}", e.what());
    }
}

void Webcam::ReadFrame(cv::Mat& frame) { capture_.read(frame); }