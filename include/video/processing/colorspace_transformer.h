/******************************************************************************
 * Filename:    colorspace_transformer.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef COLOR_SPACE_TRANSFORMER_H
#define COLOR_SPACE_TRANSFORMER_H

#include "logger.h"
#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

enum class Colorspace {
    BYPASS,
    BGR2GRAY,
    BGR2HSV,
};

class BypassTransformer : public VideoTransformer {
   public:
    void Transform(cv::Mat& frame) override;
};

class BGR2GRAYTransformer : public VideoTransformer {
   public:
    void Transform(cv::Mat& frame) override;
};

class BGR2HSVTransformer : public VideoTransformer {
   public:
    void Transform(cv::Mat& frame) override;
};

class ColorspaceTransformerFactory : public VideoTransformerFactory {
   public:
    ColorspaceTransformerFactory(Colorspace colorspace = Colorspace::BYPASS)
        : colorspace_(colorspace) {}
    std::shared_ptr<VideoTransformer> Create() override {
        if (colorspace_ == Colorspace::BYPASS) {
            return std::make_shared<BypassTransformer>();
        } else if (colorspace_ == Colorspace::BGR2GRAY) {
            return std::make_shared<BGR2GRAYTransformer>();
        } else if (colorspace_ == Colorspace::BGR2HSV) {
            return std::make_shared<BGR2HSVTransformer>();
        } else {
            spdlog::error("Invalid Colorspace Type");
            return nullptr;
        }
    }
    void SetColorspace(Colorspace colorspace) { colorspace_ = colorspace; }

   private:
    Colorspace colorspace_;
};

#endif  // COLOR_SPACE_TRANSFORMER_H
