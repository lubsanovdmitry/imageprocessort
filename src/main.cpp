#include <filesystem>
#include <iostream>
#include <ostream>

#include <catch2/catch_all.hpp>

#include "FilterController/FilterController.h"
#include "Image/Image.h"
#include "ImageFormats/BMP.h"
#include "CLI/CommandLineArgs.h"

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

    image_processor::BMP bmp;

    image_processor::Image image = bmp.Read(file_names.source);

    image = controller.Apply(image);

    bmp.Write(image, file_names.dest);

    return 0;
}
