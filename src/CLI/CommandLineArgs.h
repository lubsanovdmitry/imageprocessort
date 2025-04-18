#pragma once

#include <filesystem>
#include <sstream>

#include "FilterController/FilterController.h"
#include "ImageFormats/ImageFormat.h"

namespace image_processor::command_line {

using ArgsStream = std::stringstream;

static constexpr char Help[] =
    "Usage: image_processor input output [filters]\n"
    "Filters:\n"
    "\t-gs Grayscale\n"
    "\t-crop width height Crops to specified width and height\n"
    "\t-neg Negative\n"
    "\t-edge threshold Edge with specified threshold\n"
    "\t-blur sigma Gaussian Blur with parameter sigma\n"
    "\t-distort radius grain Perlin Noise distortion with radius and grain size\n";

class ArgsError : public std::exception {
public:
    const char* what() const throw() override {
        return Help;
    }
};

struct Filenames {
    std::filesystem::path source;
    std::filesystem::path dest;
};

template <typename T>
T Read(ArgsStream& ss);

ArgsStream ConstructArgsStream(int argc, char** argv);

Filenames GetFilenames(ArgsStream& as);

filter_controller::FilterArray GetFilters(ArgsStream& as);

std::unique_ptr<image_processor::formats::ImageFormat> GetFormat(std::filesystem::path path);

}  // namespace image_processor::command_line