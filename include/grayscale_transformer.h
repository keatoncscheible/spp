/******************************************************************************
 * Filename:    grayscale_transformer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef GRAYSCALE_TRANSFORMER_H
#define GRAYSCALE_TRANSFORMER_H

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

class GrayscaleTransformer : public VideoTransformer {
   public:
    void Open();
    void Close();
    void ReadFrame(cv::Mat& frame);
    void Transform(cv::Mat& frame) override;
};

class GrayscaleTransformerFactory : public VideoTransformerFactory {
   public:
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<GrayscaleTransformer>();
    }
};

#endif  // GRAYSCALE_TRANSFORMER_H
