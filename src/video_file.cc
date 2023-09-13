/******************************************************************************
 * Filename:    video_file.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_file.h"

#include <iostream>

#include "opencv2/core.hpp"

void VideoFile::Open() {
    std::cout << "Opening Video File: " << filename_ << std::endl;
    // Implement the logic to open a video file
}

void VideoFile::Close() {
    std::cout << "Closing Video File: " << filename_ << std::endl;
    // Implement the logic to close a video file
}

void VideoFile::ReadFrame(cv::Mat& frame) {
    std::cout << "Reading frame from Video File: " << filename_ << std::endl;
    // Implement the logic to read a frame from the video file
}