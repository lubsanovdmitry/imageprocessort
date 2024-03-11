#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace bmp_default_values {
constexpr size_t BMPHeaderSize = 14;
constexpr size_t DIBHeaderSize = 40;
constexpr int32_t DefaultOffset = 54;
constexpr int32_t DefaultBPP = 24;
constexpr int32_t DIBSize = 40;
}  // namespace bmp_default_values

struct Pixel {
    int8_t red{};
    int8_t green{};
    int8_t blue{};
};

struct BMPHeader {
    int8_t B{'B'};
    int8_t M{'M'};
    int32_t file_size{};
    int16_t res1{};
    int16_t res2{};
    int32_t offset{bmp_default_values::DefaultOffset};
};

struct DIBHeader {
    int32_t dib_size{bmp_default_values::DIBSize};
    int32_t width{};
    int32_t height{};
    int16_t planes{1};
    int16_t bits_per_pixel{bmp_default_values::DefaultBPP};
    int32_t compression{0};
    int32_t bitmap_size{};
    int32_t horizontal_resolution{};
    int32_t vertical_resolution{};
    int32_t colors_in_palette{};
    int32_t important_colors{0};
};

class ImageHandler {
public:
    ImageHandler() {
    }

    void Read(std::filesystem::path path);

    void Write(std::filesystem::path path);

private:
    BMPHeader bmp_header_{};
    DIBHeader dib_header_{};
    std::vector<std::vector<Pixel>> image_{};
    int32_t padding_{};
};