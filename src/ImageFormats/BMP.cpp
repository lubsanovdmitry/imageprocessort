#include "ImageFormats/BMP.h"

#include <cstdint>
#include <limits>
#include <vector>

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

namespace bmp_default_values {
constexpr size_t BMPHeaderSize = 14;
constexpr size_t DIBHeaderSize = 40;
constexpr uint32_t DefaultOffset = 54;
constexpr uint32_t DefaultBPP = 24;
constexpr uint32_t DIBSize = 40;
}  // namespace bmp_default_values

struct BMPHeader {
    uint8_t B{'B'};
    uint8_t M{'M'};
    uint32_t file_size{};
    uint16_t res1{};
    uint16_t res2{};
    uint32_t offset{bmp_default_values::DefaultOffset};
};

struct DIBHeader {
    uint32_t dib_size{bmp_default_values::DIBSize};
    int32_t width{};
    int32_t height{};
    uint16_t planes{1};
    uint16_t bits_per_pixel{bmp_default_values::DefaultBPP};
    uint32_t compression{0};
    uint32_t bitmap_size{};
    uint32_t horizontal_resolution{};
    uint32_t vertical_resolution{};
    uint32_t colors_in_palette{};
    uint32_t important_colors{0};
};

}  // namespace

image_processor::Image BMP::Read(std::filesystem::path path) {
    io::FileReader reader(path);
    BMPHeader bmp_header{};
    DIBHeader dib_header{};
    reader >> bmp_header.B >> bmp_header.M >> bmp_header.file_size >> bmp_header.res1 >> bmp_header.res2 >>
        bmp_header.offset;
    reader >> dib_header.dib_size >> dib_header.width >> dib_header.height >> dib_header.planes >>
        dib_header.bits_per_pixel >> dib_header.compression >> dib_header.bitmap_size >>
        dib_header.horizontal_resolution >> dib_header.vertical_resolution >> dib_header.colors_in_palette >>
        dib_header.important_colors;
    Image image(dib_header.height, image_processor::Row(dib_header.width));
    int64_t padding = (4 - (dib_header.width % 4)) % 4;
    std::vector<char> pad(padding);
    for (int64_t i = 0; i < dib_header.height; ++i) {
        for (int64_t j = 0; j < dib_header.width; ++j) {
            RawPixel raw_pixel{};
            reader >> raw_pixel;
            double red = Normalize(raw_pixel.red);
            double green = Normalize(raw_pixel.green);
            double blue = Normalize(raw_pixel.blue);
            image[i][j] = {blue, green, red};
        }
        for (int64_t i = 0; i < padding; ++i) {
            char c{};
            reader >> c;
        }
        // reader.SeekGet(padding, std::ios::cur);
    }
    return image;
}

void BMP::Write(const image_processor::Image &image, std::filesystem::path path) {
    io::FileWriter writer(path);
    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    dib_header.height = static_cast<int32_t>(image.Size());
    dib_header.width = (!image.Empty()) ? static_cast<int32_t>(image[0].Size()) : 0;
    int64_t padding = (4 - (dib_header.width * 3 % 4)) % 4;

    dib_header.bitmap_size = (dib_header.width * 3 + padding) * dib_header.height;
    bmp_header.file_size = dib_header.bitmap_size + bmp_default_values::DefaultOffset;

    writer << bmp_header.B << bmp_header.M << bmp_header.file_size << bmp_header.res1 << bmp_header.res2
           << bmp_header.offset;
    writer << dib_header.dib_size << dib_header.width << dib_header.height << dib_header.planes
           << dib_header.bits_per_pixel << dib_header.compression << dib_header.bitmap_size
           << dib_header.horizontal_resolution << dib_header.vertical_resolution << dib_header.colors_in_palette
           << dib_header.important_colors;

    std::vector<char> pad(padding);
    for (int64_t i = 0; i < dib_header.height; ++i) {
        for (int64_t j = 0; j < dib_header.width; ++j) {
            RawPixel raw_pixel{};
            raw_pixel.blue = Denormalize<uint8_t>(image[i][j].blue);
            raw_pixel.green = Denormalize<uint8_t>(image[i][j].green);
            raw_pixel.red = Denormalize<uint8_t>(image[i][j].red);
            writer << raw_pixel;
        }
        const char c{};
        for (int64_t i = 0; i < padding; ++i) {
            writer << c;
        }
        // writer.SeekPut(padding, std::ios::cur);
    }
}

}  // namespace image_processor