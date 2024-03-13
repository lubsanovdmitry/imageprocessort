#include "ImageFormats/BMP.h"

#include <concepts>
#include <cstdint>
#include <limits>

#include "IO/FileReader.h"
#include "IO/FileWriter.h"
#include "Image/Image.h"
#include "Image/Pixel.h"
#include "Utils/ClampedVector.h"

namespace image_processor {

namespace {

struct RawPixel {
    uint8_t blue{};
    uint8_t green{};
    uint8_t red{};
};

template <std::integral T>
double Normalize(T t) {
    return static_cast<double>(t) / std::numeric_limits<T>::max();
}

template <std::integral T>
T Denormalize(double d) {
    return static_cast<T>(d * std::numeric_limits<T>::max());
}

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
    size_t padding = (4 - (dib_header.width % 4)) % 4;
    for (size_t i = 0; i < dib_header.height; ++i) {
        for (size_t j = 0; j < dib_header.width; ++j) {
            RawPixel raw_pixel{};
            reader >> raw_pixel.blue >> raw_pixel.green >> raw_pixel.red;
            double red = Normalize(raw_pixel.red);
            double green = Normalize(raw_pixel.green);
            double blue = Normalize(raw_pixel.blue);
            image[i][j] = {blue, green, red};
        }
        for (size_t k = 0; k < padding; ++k) {
            char c{};
            reader >> c;
        }
    }
    return image;
}

void BMP::Write(const image_processor::Image &image, std::filesystem::path path) {
    FileWriter writer(path);
    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    dib_header.height = image.Size();
    dib_header.width = (!image.Empty()) ? image[0].Size() : 0;
    size_t padding = (4 - (dib_header.width % 4)) % 4;

    dib_header.bitmap_size = (dib_header.width * 3 + padding) * dib_header.height;
    bmp_header.file_size = dib_header.bitmap_size + bmp_default_values::DefaultOffset;

    writer << bmp_header.B << bmp_header.M << bmp_header.file_size << bmp_header.res1 << bmp_header.res2
           << bmp_header.offset;
    writer << dib_header.dib_size << dib_header.width << dib_header.height << dib_header.planes
           << dib_header.bits_per_pixel << dib_header.compression << dib_header.bitmap_size
           << dib_header.horizontal_resolution << dib_header.vertical_resolution << dib_header.colors_in_palette
           << dib_header.important_colors;
    for (size_t i = 0; i < dib_header.height; ++i) {
        for (size_t j = 0; j < dib_header.width; ++j) {
            RawPixel raw_pixel{};
            raw_pixel.blue = Denormalize<uint8_t>(image[i][j].blue);
            raw_pixel.green = Denormalize<uint8_t>(image[i][j].green);
            raw_pixel.red = Denormalize<uint8_t>(image[i][j].red);
            writer << raw_pixel;
        }
        for (size_t k = 0; k < padding; ++k) {
            char c{};
            writer << c;
        }
    }
}

}  // namespace image_processor