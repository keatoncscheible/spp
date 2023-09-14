/******************************************************************************
 * Filename:    video_player.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#include "opencv2/highgui.hpp"
#include "video_consumer.h"

class VideoPlayer : public VideoConsumer {
   public:
    VideoPlayer(const std::string& windowName);
    void Consume(const cv::Mat& frame) override;

   private:
    std::string windowName_;
};

class VideoPlayerFactory : public VideoConsumerFactory {
   public:
    VideoPlayerFactory(const std::string& windowName)
        : windowName_(windowName) {}
    std::shared_ptr<VideoConsumer> Create() override {
        return std::make_shared<VideoPlayer>(windowName_);
    }

   private:
    std::string windowName_;
};
