#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace bmp_default_values {
constexpr size_t BMPHeaderSize = 14;
constexpr size_t DIBHeaderSize = 40;
constexpr uint32_t DefaultOffset = 54;
constexpr uint32_t DefaultBPP = 24;
constexpr uint32_t DIBSize = 40;
}  // namespace bmp_default_values

struct RawPixel {
    uint8_t red{};
    uint8_t green{};
    uint8_t blue{};
};

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
    uint32_t width{};
    uint32_t height{};
    uint16_t planes{1};
    uint16_t bits_per_pixel{bmp_default_values::DefaultBPP};
    uint32_t compression{0};
    uint32_t bitmap_size{};
    uint32_t horizontal_resolution{};
    uint32_t vertical_resolution{};
    uint32_t colors_in_palette{};
    uint32_t important_colors{0};
};

class BMP {
public:
    BMP() {
    }

    void Read(std::filesystem::path path);

    void Write(std::filesystem::path path);

private:
    BMPHeader bmp_header_{};
    DIBHeader dib_header_{};
    std::vector<std::vector<RawPixel>> image_{};
    int32_t padding_{};
};