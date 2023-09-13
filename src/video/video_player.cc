/******************************************************************************
 * Filename:    video_input.cc
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "video_player.h"

VideoPlayer::VideoPlayer(const std::string& windowName)
    : windowName_(windowName) {
    cv::namedWindow(windowName_, cv::WINDOW_AUTOSIZE);
}

void VideoPlayer::Consume(const cv::Mat& frame) {
    cv::imshow(windowName_, frame);
    cv::waitKey(1);  // Waits for 1 millisecond between frames.
}
