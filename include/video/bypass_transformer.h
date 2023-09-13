/******************************************************************************
 * Filename:    bypass_transformer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef BYPASS_TRANSFORMER_H
#define BYPASS_TRANSFORMER_H

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

class BypassTransformer : public VideoTransformer {
   public:
    void Transform(cv::Mat& frame) override;
};

class BypassTransformerFactory : public VideoTransformerFactory {
   public:
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<BypassTransformer>();
    }
};

#endif  // BYPASS_TRANSFORMER_H
