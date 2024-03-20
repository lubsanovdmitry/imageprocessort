#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>

#include <catch2/catch_all.hpp>

#include "FilterController/FilterController.h"
#include "Image/Image.h"
#include "ImageFormats/BMP.h"
#include "CLI/CommandLineArgs.h"
#include "ImageFormats/ImageFormat.h"

int main(int argc, char* argv[]) {
    image_processor::command_line::ArgsStream args_stream =
        image_processor::command_line::ConstructArgsStream(argc, argv);
    image_processor::command_line::Filenames file_names{};

    try {
        file_names = image_processor::command_line::GetFilenames(args_stream);
    } catch (const image_processor::command_line::ArgsError& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    image_processor::filter_controller::FilterController controller;

    try {
        controller = image_processor::filter_controller::FilterController(
            image_processor::command_line::GetFilters(args_stream));
    } catch (const image_processor::command_line::ArgsError& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    std::unique_ptr<image_processor::IImageFormat> format = std::make_unique<image_processor::BMP>();

    image_processor::Image image = format->Read(file_names.source);

    image = controller.Apply(image);

    std::cerr << "\033[1;31m " << file_names.source.string() << "\033[0m" << std::endl;

    std::cerr << "\033[1;31m " << file_names.dest.string() << "\033[0m" << std::endl;

    format->Write(image, file_names.dest);

    return 0;
}
