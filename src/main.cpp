#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <system_error>
#include <vector>

#include <catch2/catch_all.hpp>

#include "Filters/BaseFilter.h"
#include "FilterController/FilterController.h"
#include "Filters/Factory.h"
#include "IO/FileReader.h"
#include "IO/FileWriter.h"
#include "Utils/ClampedVector.h"
#include "Image/Image.h"
#include "ImageFormats/BMP.h"
#include "Filters/Filters.h"
#include "CLI/CommandLineArgs.h"

/*image_processor::filter_controller::FilterArray SendFilters(image_processor::command_line::ArgsStream& ss) {
    image_processor::filter_controller::FilterArray fa;
    // fa.push_back(image_processor::filters::Registry::GetRegistry().at("-gs")(ss));
    // fa.push_back(image_processor::filters::Registry::GetRegistry().at("-neg")(ss));
    fa.push_back(image_processor::filters::Registry::GetRegistry().at("-neg")(ss));

    // fa.push_back(std::make_unique<image_processor::filters::Grayscale>());
    return fa;
}*/

int main(int argc, char* argv[]) {
    image_processor::command_line::ArgsStream args_stream =
        image_processor::command_line::ConstructArgsStream(argc, argv);
    image_processor::command_line::Filenames file_names{};

    try {
        // std::cerr << std::filesystem::current_path();
        file_names = image_processor::command_line::GetFilenames(args_stream);
    } catch (const image_processor::command_line::ArgsError& e) {
        std::cerr << e.what() << std::endl;
    }

    // SendFilters(args_stream);

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
