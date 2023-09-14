/******************************************************************************
 * haar_cascades_filename:    haar_cascade_classifier.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef HAAR_CASCADE_CLASSIFIER_H
#define HAAR_CASCADE_CLASSIFIER_H

#include <map>

#include "opencv2/imgproc.hpp"
#include "video_transformer.h"

#define HAAR_CASCADE_CLASSIFIER_PATH "assets/data/haarcascades/"

enum class HaarCascadeClassifierType {
    Eyes,
    LeftEye,
    RightEye,
    EyesWithGlasses,
    FrontalFace,
    FrontalFaceAlt,
    FrontalFaceAlt2,
    FrontalFaceAltTree,
    ProfileFace,
    Smile,
    FullBody,
    UpperBody,
    LowerBody,
    CatFrontalFace,
    CatFrontalFaceExtended,
    RussianPlateNumber,
    LicensePlateRus16Stages,
};

class HaarCascadeClassifier : public VideoTransformer {
   public:
    HaarCascadeClassifier(const std::string haar_cascades_filename)
        : haar_cascades_filename_(haar_cascades_filename){};
    void Transform(cv::Mat& frame) override;

   private:
    std::string haar_cascades_filename_;
};

class HaarCascadeClassifierFactory : public VideoTransformerFactory {
   public:
    HaarCascadeClassifierFactory(
        HaarCascadeClassifierType haar_cascade_classifier_type)
        : haar_cascade_classifier_type_(haar_cascade_classifier_type) {}
    std::shared_ptr<VideoTransformer> Create() override {
        return std::make_shared<HaarCascadeClassifier>(
            haar_cascade_classifier_file_map_[haar_cascade_classifier_type_]);
    }

   private:
    HaarCascadeClassifierType haar_cascade_classifier_type_;
    std::map<HaarCascadeClassifierType, std::string>
        haar_cascade_classifier_file_map_ = {
            {HaarCascadeClassifierType::Eyes,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_eye.xml"},
            {HaarCascadeClassifierType::LeftEye,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_lefteye_2splits.xml"},
            {HaarCascadeClassifierType::RightEye,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_righteye_2splits.xml"},
            {HaarCascadeClassifierType::EyesWithGlasses,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_eye_tree_eyeglasses.xml"},
            {HaarCascadeClassifierType::FrontalFace,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_frontalface_default.xml"},
            {HaarCascadeClassifierType::FrontalFaceAlt,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_frontalface_alt.xml"},
            {HaarCascadeClassifierType::FrontalFaceAlt2,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_frontalface_alt2.xml"},
            {HaarCascadeClassifierType::FrontalFaceAltTree,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_frontalface_alt_tree.xml"},
            {HaarCascadeClassifierType::ProfileFace,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_profileface.xml"},
            {HaarCascadeClassifierType::Smile,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_smile.xml"},
            {HaarCascadeClassifierType::FullBody,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_fullbody.xml"},
            {HaarCascadeClassifierType::UpperBody,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_upperbody.xml"},
            {HaarCascadeClassifierType::LowerBody,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_lowerbody.xml"},
            {HaarCascadeClassifierType::CatFrontalFace,
             HAAR_CASCADE_CLASSIFIER_PATH "haarcascade_catfrontalface.xml"},
            {HaarCascadeClassifierType::CatFrontalFaceExtended,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_catfrontalface_extended.xml"},
            {HaarCascadeClassifierType::RussianPlateNumber,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_russian_plate_number.xml"},
            {HaarCascadeClassifierType::LicensePlateRus16Stages,
             HAAR_CASCADE_CLASSIFIER_PATH
             "haarcascade_license_plate_rus_16stages.xml"},
        };
};

#endif  // HAAR_CASCADE_CLASSIFIER_H
