#include "CLI/CommandLineArgs.h"

#include <exception>
#include <iostream>
#include <stdexcept>

#include "FilterController/FilterController.h"
#include "Filters/Factory.h"
#include "ImageFormats/ImageFormat.h"

namespace image_processor::command_line {

template <>
int Read<int>(ArgsStream& ss) {
    if (ss.bad()) {
        throw std::runtime_error{"No parameters provided."};
    }
    std::string s;
    ss >> s;
    int conv{};
    try {
        conv = std::stoi(s);
    } catch (std::invalid_argument ia) {
        throw std::invalid_argument{"Invalid parameter to filter."};
    }
    return conv;
}

template <>
int64_t Read<int64_t>(ArgsStream& ss) {
    if (ss.bad()) {
        throw std::runtime_error{"No parameters provided."};
    }
    int conv{};
    try {
        ss >> conv;
    } catch (std::invalid_argument ia) {
        throw std::invalid_argument{"Invalid parameter to filter."};
    }
    return conv;
}

template <>
double Read<double>(ArgsStream& ss) {
    if (ss.bad()) {
        throw std::runtime_error{"No parameters provided."};
    }
    std::string s;
    ss >> s;
    double conv{};
    try {
        conv = std::stod(s);
    } catch (std::invalid_argument ia) {
        throw std::invalid_argument{"Invalid parameter to filter."};
    }
    return conv;
}

ArgsStream ConstructArgsStream(int argc, char* argv[]) {
    std::stringstream ss;
    for (int i = 1; i < argc; ++i) {
        ss << argv[i] << ' ';
    }
    return ss;
}

Filenames GetFilenames(ArgsStream& as) {
    Filenames file_names;

    try {
        as >> file_names.source;
        as >> file_names.dest;
        if (!std::filesystem::exists(file_names.source)) {
            throw std::runtime_error{"Could not open file: " + file_names.source.string()};
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        throw ArgsError{};
    }

    return file_names;
}

filter_controller::FilterArray GetFilters(ArgsStream& as) {
    filter_controller::FilterArray filters;
    std::string name;
    while (as >> name) {
        try {
            filters.emplace_back(filters::Registry::GetRegistry().at(name)(as));
        } catch (const std::out_of_range& e) {
            std::cerr << "Invalid filter or argument name: " << name << std::endl;
            throw ArgsError{};
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            throw ArgsError{};
        }
    }
    return filters;
}

std::unique_ptr<image_processor::formats::ImageFormat> GetFormat(std::filesystem::path path) {
    std::string name;
    try {
        name = path.string().substr(path.string().rfind('.'));
        return formats::Registry::GetRegistry().at(name)();
    } catch (const std::out_of_range& e) {
        std::cerr << "Incorrect or unsupported image format: " << name << std::endl;
        throw ArgsError{};
    }
}

}  // namespace image_processor::command_line