#pragma once

#include <cstddef>
#include "Filters/BaseFilter.h"

namespace image_processor::filters {

class Crop : public Filter {
public:
    Crop() : new_height_(0), new_width_(0) {
    }

    Crop(size_t new_height, size_t new_width) : new_height_(new_height), new_width_(new_width) {
    }

    Image Apply(const Image& image) override;

private:
    size_t new_height_;
    size_t new_width_;
};

class Grayscale : public Filter {
private:
    static constexpr double Red = 0.299;
    static constexpr double Green = 0.587;
    static constexpr double Blue = 0.114;

public:
    Image Apply(const Image& image) override;
};

class Negative : public Filter {
public:
    Image Apply(const Image& image) override;
};

class Sharpening : public ConvolutionFilter {
public:
    Image Apply(const Image& image) override;

private:
    static constexpr std::array<std::array<double, 3>, 3> ConvolutionMatrix{{{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}};
};

class EdgeDetection : public ConvolutionFilter {
public:
    Image Apply(const Image& image) override;

private:
    static constexpr std::array<std::array<double, 3>, 3> ConvolutionMatrix{{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}};
};

class Gaussian : public Filter {
public:
    Image Apply(const Image& image) override;
};

}  // namespace image_processor::filters