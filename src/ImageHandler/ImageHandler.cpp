#include "ImageHandler/ImageHandler.h"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

namespace {
size_t GetFileSize(std::ifstream& file) {
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    return file_size;
}
}  // anonymous namespace

void ImageHandler::Read(std::filesystem::path path) {
    std::ifstream image(path, std::ios::in | std::ios::binary);
    if (!image.is_open()) {
        throw "failure opening file";
    }
    size_t file_size = GetFileSize(image);
    if (file_size < bmp_default_values::DIBHeaderSize + bmp_default_values::BMPHeaderSize) {
        throw "corrupted file";
    }
    image.read(reinterpret_cast<char*>(&bmp_header_.B), sizeof(bmp_header_.B));
    image.read(reinterpret_cast<char*>(&bmp_header_.M), sizeof(bmp_header_.M));
    image.read(reinterpret_cast<char*>(&bmp_header_.file_size), sizeof(bmp_header_.file_size));
    image.read(reinterpret_cast<char*>(&bmp_header_.res1), sizeof(bmp_header_.res1));
    image.read(reinterpret_cast<char*>(&bmp_header_.res2), sizeof(bmp_header_.res2));
    image.read(reinterpret_cast<char*>(&bmp_header_.offset), sizeof(bmp_header_.offset));
    if (bmp_header_.B != 'B' || bmp_header_.M != 'M') {
        throw "incorrect file";
    }

    image.read(reinterpret_cast<char*>(&dib_header_.dib_size), sizeof(dib_header_.dib_size));
    image.read(reinterpret_cast<char*>(&dib_header_.width), sizeof(dib_header_.width));
    image.read(reinterpret_cast<char*>(&dib_header_.height), sizeof(dib_header_.height));
    image.read(reinterpret_cast<char*>(&dib_header_.planes), sizeof(dib_header_.planes));
    image.read(reinterpret_cast<char*>(&dib_header_.bits_per_pixel), sizeof(dib_header_.bits_per_pixel));
    image.read(reinterpret_cast<char*>(&dib_header_.compression), sizeof(dib_header_.compression));
    image.read(reinterpret_cast<char*>(&dib_header_.bitmap_size), sizeof(dib_header_.bitmap_size));
    image.read(reinterpret_cast<char*>(&dib_header_.horizontal_resolution), sizeof(dib_header_.horizontal_resolution));
    image.read(reinterpret_cast<char*>(&dib_header_.vertical_resolution), sizeof(dib_header_.vertical_resolution));
    image.read(reinterpret_cast<char*>(&dib_header_.colors_in_palette), sizeof(dib_header_.colors_in_palette));
    image.read(reinterpret_cast<char*>(&dib_header_.important_colors), sizeof(dib_header_.important_colors));

    if (dib_header_.bits_per_pixel != bmp_default_values::DefaultBPP) {
        throw "unsupported image format";
    }

    padding_ = 4 - (dib_header_.width * 3) % 4;

    int32_t row_size = dib_header_.width * 3 + padding_;

    image_.resize(dib_header_.height, std::vector<Pixel>(dib_header_.width));

    for (int32_t i = 0; i < dib_header_.height; ++i) {
        image.seekg(bmp_header_.offset + row_size * i);
        for (Pixel& p : image_[i]) {
            image.read(reinterpret_cast<char*>(&p), sizeof(p));
        }
    }
}