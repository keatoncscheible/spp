/******************************************************************************
 * haar_cascades_filename:    object_detector.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef OBJECT_DETECTOR_H
#define OBJECT_DETECTOR_H

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

class ObjectDetector : public VideoTransformer {
   public:
    ObjectDetector(const std::string& haar_cascades_filename)
        : haar_cascades_filename_(haar_cascades_filename){};
    void Transform(cv::Mat& frame) override;

   private:
    std::string haar_cascades_filename_;
};

class ObjectDetectorFactory : public VideoTransformerFactory {
   public:
    ObjectDetectorFactory(const std::string& haar_cascades_filename)
        : haar_cascades_filename_(haar_cascades_filename) {}
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<ObjectDetector>(haar_cascades_filename_);
    }

   private:
    std::string haar_cascades_filename_;
};

#endif  // OBJECT_DETECTOR_H
