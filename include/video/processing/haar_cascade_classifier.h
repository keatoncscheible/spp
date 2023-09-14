/******************************************************************************
 * haar_cascades_filename:    haar_cascade_classifier.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef HAAR_CASCADE_CLASSIFIER_H
#define HAAR_CASCADE_CLASSIFIER_H

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

class HaarCascadeClassifier : public VideoTransformer {
   public:
    HaarCascadeClassifier(const std::string& haar_cascades_filename)
        : haar_cascades_filename_(haar_cascades_filename){};
    void Transform(cv::Mat& frame) override;

   private:
    std::string haar_cascades_filename_;
};

class HaarCascadeClassifierFactory : public VideoTransformerFactory {
   public:
    HaarCascadeClassifierFactory(const std::string& haar_cascades_filename)
        : haar_cascades_filename_(haar_cascades_filename) {}
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<HaarCascadeClassifier>(haar_cascades_filename_);
    }

   private:
    std::string haar_cascades_filename_;
};

#endif  // HAAR_CASCADE_CLASSIFIER_H
