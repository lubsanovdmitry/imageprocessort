#pragma once

#include <filesystem>
#include <functional>

#include "Image/Image.h"

namespace image_processor::formats {

class ImageFormat {
public:
    ImageFormat() = default;
    virtual ~ImageFormat() = default;

    virtual image_processor::Image Read(std::filesystem::path path) = 0;

    virtual void Write(const image_processor::Image& image, std::filesystem::path path) = 0;
};

class Registry {
public:
    using FormatFactory = std::function<std::unique_ptr<ImageFormat>()>;
    using RegistryMap = std::unordered_map<std::string, FormatFactory>;

    static RegistryMap& GetRegistry() {
        static RegistryMap registry;
        return registry;
    }

private:
    Registry() = default;
};

template <typename T>
struct RegisterFormat {
    explicit RegisterFormat(std::string name) {
        auto r = [t = T()]() -> std::unique_ptr<ImageFormat> { return t(); };
        Registry::GetRegistry()[name] = r;
    }
};

struct BaseImageFormatFactory {
    virtual std::unique_ptr<ImageFormat> operator()() const = 0;
};

}  // namespace image_processor::formats