/******************************************************************************
 * Filename:    webcam.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "webcam.h"

#include "logger.h"
#include "opencv2/core.hpp"

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