#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>

#include "Image/Pixel.h"

namespace image {

namespace {

class Row {
public:
    Row() = default;

    Row(size_t size) : row_(size) {
    }

    void Resize(size_t new_size) {
        row_.resize(new_size);
    }

    size_t GetWidth() const noexcept {
        return row_.size();
    }

    const Pixel& operator[](size_t i) const {
        return row_[std::clamp(i, 0ul, GetWidth() - 1)];
    }

    Pixel& operator[](size_t i) {
        return row_[std::clamp(i, 0ul, GetWidth() - 1)];
    }

private:
    std::vector<Pixel> row_;
};

}  // namespace

class Image {
private:
public:
    Image() = default;

    Image(size_t height, size_t width) : height_(height), width_(width), image_(height, Row(width)) {
    }

    size_t GetHeight() const {
        return height_;
    }

    size_t GetWidth() const {
        return width_;
    };

    void ChangeHeight(size_t new_height) {
        image_.resize(new_height, Row(width_));
        height_ = new_height;
    }

    void ChangeWidth(size_t new_width) {
        for (Row& r : image_) {
            r.Resize(new_width);
        }
        width_ = new_width;
    }

    const Row& operator[](size_t i) const {
        return image_[std::clamp(i, 0ul, image_.size() - 1)];
    }

    Row& operator[](size_t i) {
        return image_[std::clamp(i, 0ul, image_.size() - 1)];
    }

private:
    size_t height_{};
    size_t width_{};

    std::vector<Row> image_{};
};

}  // namespace image