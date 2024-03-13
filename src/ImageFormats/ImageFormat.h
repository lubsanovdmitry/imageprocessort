#pragma once

#include <filesystem>

#include "Image/Image.h"

namespace image_processor {

class IImageFormat {
public:
    IImageFormat() = default;
    virtual ~IImageFormat() = default;

    virtual image_processor::Image Read(std::filesystem::path path) = 0;

    virtual void Write(const image_processor::Image& image, std::filesystem::path path) = 0;
};

}  // namespace image_processor