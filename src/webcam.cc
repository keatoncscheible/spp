/******************************************************************************
 * Filename:    webcam.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "webcam.h"

#include "opencv2/core.hpp"

void Webcam::Open() {
    std::cout << "Opening webcam" << std::endl;
    if (capture_.isOpened()) {
        std::cout << "Webcam already opened" << std::endl;
        return;
    }
    capture_.open(0);
    if (!capture_.isOpened()) {
        std::cerr << "Problem opening webcam" << std::endl;
    }
}

void Webcam::Close() {
    std::cout << "Closing webcam" << std::endl;
    try {
        if (capture_.isOpened()) {
            capture_.release();
        }
    } catch (const std::exception& e) {
        std::string error_message =
            "Error stopping video capture: " + std::string(e.what());
        std::cerr << error_message << std::endl;
    }
}

void Webcam::ReadFrame(cv::Mat& frame) { capture_.read(frame); }