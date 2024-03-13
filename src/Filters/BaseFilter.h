#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "Image/Image.h"

namespace image_processor::filters {

class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;

    virtual Image Apply(const Image& image) = 0;
};

class ConvolutionFilter : public Filter {
private:
    struct Index {
        int64_t x;
        int64_t y;
    };

    static constexpr std::array<Index, 9> IndexArray{
        {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

    static constexpr std::array<std::array<double, 3>, 3> ConvolutionMatrix{{{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}};
};

template <typename F>
concept FilterFactory =
    std::is_default_constructible_v<F> && std::invocable<F, const std::vector<std::string>&> &&
    std::is_convertible_v<std::invoke_result_t<F, const std::vector<std::string>&>, std::unique_ptr<Filter> >;

}  // namespace image_processor::filters
