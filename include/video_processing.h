/******************************************************************************
 * Filename:    video_processing.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef VIDEO_PROCESSING_H
#define VIDEO_PROCESSING_H

#include <condition_variable>
#include <mutex>
#include <opencv2/opencv.hpp>

#include "statistics.h"
#include "task.h"
#include "video_capture.h"

class VideoProcessing {
   public:
    VideoProcessing(VideoCapture& video_capture_);
    ~VideoProcessing();
    void Start() { task_.Start(); }
    void Join() { task_.Join(); }
    cv::Mat& GetOutputFrame() { return current_buffer_; }
    StatisticsQueue<double> time_stats_{100};
    std::mutex mutex_;
    std::condition_variable cond_;

   private:
    static void VideoProcessingFunction(Task* task);
    void GetInputFrame(cv::Mat& frame);
    void ProcessVideo(cv::Mat& frame_in, cv::Mat& frame_out);
    void SwapBuffers();
    void NotifyListeners();
    Task task_;
    VideoCapture& video_capture_;
    cv::Mat buffer1_, buffer2_;
    cv::Mat& current_buffer_;
    cv::Mat& next_buffer_;
};

#endif  // VIDEO_PROCESSING_H
