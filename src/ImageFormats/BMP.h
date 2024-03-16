#pragma once

#include <filesystem>

#include "Image/Image.h"
#include "ImageFormats/ImageFormat.h"

namespace image_processor {

class BMP : public IImageFormat {
public:
    image_processor::Image Read(std::filesystem::path path) override;
    void Write(const image_processor::Image& image, std::filesystem::path path) override;
};

}  // namespace image_processor