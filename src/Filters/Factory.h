#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Filters/BaseFilter.h"

namespace image_processor::filters {

class AbstractFilterFactory {
public:
    AbstractFilterFactory() = default;
    virtual ~AbstractFilterFactory() = default;

    std::unique_ptr<Filter> operator()(const std::vector<std::string>& parameters) {
        return Construct(parameters);
    }

    virtual std::unique_ptr<Filter> Construct(const std::vector<std::string>& parameters) = 0;
};

}  // namespace image_processor::filters
