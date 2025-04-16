#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>

#include "FilterController/FilterController.h"
#include "Image/Image.h"
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

    try {
        std::unique_ptr<image_processor::formats::ImageFormat> format_read =
            image_processor::command_line::GetFormat(file_names.source);

        std::unique_ptr<image_processor::formats::ImageFormat> format_write =
            image_processor::command_line::GetFormat(file_names.dest);

        image_processor::filter_controller::FilterController controller =
            image_processor::filter_controller::FilterController(
                image_processor::command_line::GetFilters(args_stream));

        image_processor::Image image = format_read->Read(file_names.source);

        image = controller.Apply(image);

        format_write->Write(image, file_names.dest);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    // std::cerr << "\033[1;31m " << file_names.source.string() << "\033[0m" << std::endl;

    // std::cerr << "\033[1;31m " << file_names.dest.string() << "\033[0m" << std::endl;

    return 0;
}
