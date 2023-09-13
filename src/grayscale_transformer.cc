/******************************************************************************
 * Filename:    grayscale_transformer.cpp
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "grayscale_transformer.h"

#include <iostream>

void GrayscaleTransformer::Transform(cv::Mat& frame) {
    // Check if the frame has multiple channels (e.g., a color image)
    if (frame.channels() > 1) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    }
    // If the frame is already grayscale, do nothing
}

void GrayscaleTransformer::Open() {}

void GrayscaleTransformer::Close() {}

void GrayscaleTransformer::ReadFrame(cv::Mat& frame) {
    std::cout << "Reading frame from Grayscale Transformer..." << std::endl;
}