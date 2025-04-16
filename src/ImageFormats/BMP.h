#pragma once

#include <filesystem>

#include "Image/Image.h"
#include "ImageFormats/ImageFormat.h"

namespace image_processor::formats {

class BMP24 : public ImageFormat {
public:
    image_processor::Image Read(std::filesystem::path path) override;
    void Write(const image_processor::Image& image, std::filesystem::path path) override;
};

struct BMP24Factory : public BaseImageFormatFactory {
    std::unique_ptr<ImageFormat> operator()() const override {
        return std::make_unique<BMP24>();
    }
};

namespace {

const RegisterFormat<BMP24Factory> REG_BMP24(".bmp");

}  // namespace

}  // namespace image_processor::formats