/******************************************************************************
 * Filename:    video_file.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_file.h"

#include <iostream>

#include "logger.h"
#include "opencv2/core.hpp"

void VideoFile::Open() {
    spdlog::info("Opening Video File: {}", filename_);
    // Implement the logic to open a video file
}

void VideoFile::Close() {
    spdlog::info("Closing Video File: {}", filename_);
    // Implement the logic to close a video file
}

void VideoFile::ReadFrame(cv::Mat& frame) {
    spdlog::info("Reading frame from Video File: {}", filename_);
    // Implement the logic to read a frame from the video file
}