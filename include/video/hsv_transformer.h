/******************************************************************************
 * Filename:    hsv_transformer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef HSV_TRANSFORMER_H
#define HSV_TRANSFORMER_H

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

class HsvTransformer : public VideoTransformer {
   public:
    void Transform(cv::Mat& frame) override;
};

class HsvTransformerFactory : public VideoTransformerFactory {
   public:
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<HsvTransformer>();
    }
};

#endif  // HSV_TRANSFORMER_H
