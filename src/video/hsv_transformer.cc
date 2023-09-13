/******************************************************************************
 * Filename:    hsv_transformer.cpp
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include <iostream>

#include "hsv_transformer.h"

void HsvTransformer::Transform(cv::Mat& frame) {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
}
