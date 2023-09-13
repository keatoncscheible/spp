/******************************************************************************
 * Filename:    video_file.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_file.h"

#include <spdlog/spdlog.h>

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
