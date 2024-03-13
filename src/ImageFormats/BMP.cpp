#include "ImageFormats/BMP.h"
#include <cstdint>
#include <limits>

#include "Image/Image.h"
#include "IO/FileReader.h"
#include "IO/FileWriter.h"
#include "Image/Pixel.h"
#include "Utils/ClampedVector.h"

namespace image_processor {

namespace {

struct RawPixel {
    uint8_t blue{};
    uint8_t green{};
    uint8_t red{};
};

}  // namespace

image_processor::Image BMP::Read(std::filesystem::path path) {
    FileReader reader(path);
    BMPHeader bmp_header{};
    DIBHeader dib_header{};
    reader >> bmp_header.B >> bmp_header.M >> bmp_header.file_size >> bmp_header.res1 >> bmp_header.res2 >>
        bmp_header.offset;
    reader >> dib_header.dib_size >> dib_header.width >> dib_header.height >> dib_header.planes >>
        dib_header.bits_per_pixel >> dib_header.compression >> dib_header.bitmap_size >>
        dib_header.horizontal_resolution >> dib_header.vertical_resolution >> dib_header.colors_in_palette >>
        dib_header.important_colors;
    Image image(dib_header.height, image_processor::Row(dib_header.width));
    size_t padding = 4 - (dib_header.width % 4);
    for (size_t i = 0; i < dib_header.height; ++i) {
        for (size_t j = 0; j < dib_header.width; ++j) {
            RawPixel raw_pixel{};
            reader >> raw_pixel;
            double red = static_cast<double>(raw_pixel.red) / std::numeric_limits<uint8_t>::max();
            double green = static_cast<double>(raw_pixel.green) / std::numeric_limits<uint8_t>::max();
            double blue = static_cast<double>(raw_pixel.blue) / std::numeric_limits<uint8_t>::max();
            image[i][j] = Pixel{blue, red, green};
        }
        for (size_t k = 0; k < padding; ++k) {
            char c{};
            reader >> c;
        }
    }
    return image;
}

void BMP::Write(const image_processor::Image &image, std::filesystem::path path) {
}

}  // namespace image_processor