#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#include "Filters/BaseFilter.h"

namespace image_processor::filters {

class Registry {
public:
    using FilterFactory = std::function<std::unique_ptr<BaseFilter>(std::stringstream&)>;
    using RegistryMap = std::unordered_map<std::string, FilterFactory>;

public:
    static RegistryMap& GetRegistry() {
        static RegistryMap registry;
        return registry;
    }

private:
    Registry() = default;

    Registry(Registry&) = delete;

    Registry& operator=(Registry&) = delete;
};

template <typename T>
struct RegisterFilter {
    explicit RegisterFilter(std::string name) {
        auto r = [t = T()](std::stringstream& ss) -> std::unique_ptr<BaseFilter> { return t(ss); };
        Registry::GetRegistry()[name] = r;
    }
};

struct BaseFilterFactory {
    virtual std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const = 0;
};

}  // namespace image_processor::filters
