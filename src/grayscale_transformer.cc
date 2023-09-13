/******************************************************************************
 * Filename:    grayscale_transformer.cpp
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "grayscale_transformer.h"

#include <iostream>

void GrayscaleTransformer::Transform(cv::Mat& frame) {
    if (frame.channels() > 1) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    }
}
