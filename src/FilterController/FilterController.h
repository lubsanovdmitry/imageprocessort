#pragma once

#include <cstdio>
#include <memory>
#include <vector>

#include "Filters/BaseFilter.h"
#include "Image/Image.h"

namespace image_processor::filter_controller {

using FilterArray = std::vector<std::unique_ptr<image_processor::filters::BaseFilter>>;

class FilterController {
public:
    FilterController() = default;

    explicit FilterController(FilterArray&& filter_array) : filter_array_(std::move(filter_array)) {
    }

    FilterController(const FilterController&) = delete;

    FilterController& operator=(const FilterController&) = delete;

    FilterController(FilterController&& other) {
        std::swap(filter_array_, other.filter_array_);
    }

    FilterController& operator=(FilterController&& other) {
        std::swap(filter_array_, other.filter_array_);
        return *this;
    }

    ~FilterController() = default;

    image_processor::Image Apply(const Image& image);

private:
    FilterArray filter_array_;
};

}  // namespace image_processor::filter_controller