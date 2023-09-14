/******************************************************************************
 * Filename:    colorspace_transformer.cpp
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "colorspace_transformer.h"

#include <iostream>

void BypassTransformer::Transform(cv::Mat& frame) {
    // Bypassing transformation
}

void BGR2GRAYTransformer::Transform(cv::Mat& frame) {
    if (frame.channels() > 1) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    }
}

void BGR2HSVTransformer::Transform(cv::Mat& frame) {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
}
