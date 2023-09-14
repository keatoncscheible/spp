/******************************************************************************
 * Filename:    object_detector.cpp
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "object_detector.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include "logger.h"

void ObjectDetector::Transform(cv::Mat& frame) {
    if (frame.empty()) {
        spdlog::debug("ObjectDetector: empty frame");
    }

    // Load the Haar cascade for face detection
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(haar_cascades_filename_)) {
        spdlog::error("Failed to load the Haar cascade!");
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

    // Draw rectangles around the detected faces
    for (size_t i = 0; i < faces.size(); i++) {
        cv::rectangle(frame, faces[i], cv::Scalar(255, 0, 0), 2);
    }
}
