#pragma once

#include <array>
#include <cstdint>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

#include "Image/Image.h"

namespace image_processor::filters {

class BaseFilter {
public:
    BaseFilter() = default;
    virtual ~BaseFilter() = default;

    virtual Image Apply(const Image& image) = 0;
};

class BaseConvolutionFilter : public BaseFilter {
protected:
    struct Index {
        int64_t x;
        int64_t y;
    };

    static constexpr std::array<Index, 9> IndexArray{
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    using ConvolutionMatrix = std::array<std::array<double, 3>, 3>;

    virtual Image ApplyMatrix(const Image& image, const ConvolutionMatrix& cm);
};

}  // namespace image_processor::filters
