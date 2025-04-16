#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>

#include "Filters/BaseFilter.h"
#include "Filters/Factory.h"
#include "CLI/CommandLineArgs.h"
#include "Image/Image.h"

namespace image_processor::filters {

class Crop : public BaseFilter {
public:
    Crop() : new_width_(0), new_height_(0) {
    }

    Crop(size_t new_width, size_t new_height) : new_width_(new_width), new_height_(new_height) {
    }

    Image Apply(const Image& image) override;

private:
    size_t new_width_;
    size_t new_height_;
};

struct CropFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        int new_width = image_processor::command_line::Read<int>(argsstream);
        int new_height = image_processor::command_line::Read<int>(argsstream);
        return std::make_unique<Crop>(new_width, new_height);
    }
};

class Grayscale : public BaseFilter {
private:
    static constexpr double Red = 0.299;
    static constexpr double Green = 0.587;
    static constexpr double Blue = 0.114;

public:
    Image Apply(const Image& image) override;
};

struct GrayscaleFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        return std::make_unique<Grayscale>();
    }

    /*std::unique_ptr<BaseFilter> Create(std::stringstream& argsstream) const override {
        return std::make_unique<Grayscale>();
    }*/
};

class Negative : public BaseFilter {
public:
    Image Apply(const Image& image) override;
};

struct NegativeFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        return std::make_unique<Negative>();
    }
};

class Sharpening : public BaseConvolutionFilter {
public:
    Image Apply(const Image& image) override;

protected:
    static constexpr ConvolutionMatrix ConvMatrix{{{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}};
    // static constexpr std::array<std::array<double, 3>, 3> ConvolutionMatrix{{{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}};
};

struct SharpeningFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        return std::make_unique<Sharpening>();
    }
};

class EdgeDetection : public BaseConvolutionFilter {
public:
    explicit EdgeDetection(double threshold) : threshold_(threshold) {
    }

    Image Apply(const Image& image) override;

private:
    double threshold_ = 0;

    static constexpr ConvolutionMatrix ConvMatrix{{{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}};
};

struct EdgeDetectionFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        double threshold = command_line::Read<double>(argsstream);
        return std::make_unique<EdgeDetection>(threshold);
    }
};

class Gaussian : public BaseFilter {
public:
    explicit Gaussian(double sigma) : sigma_(sigma) {
    }

    Image Apply(const Image& image) override;

private:
    double sigma_ = 0;

    static constexpr int64_t Radius = 25;

private:
    double GaussDistrib(double x) const;
};

struct GaussFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        double threshold = command_line::Read<double>(argsstream);
        return std::make_unique<Gaussian>(threshold);
    }
};

class GlassDistortion : public BaseFilter {
public:
    explicit GlassDistortion(double radius, double ofs) : radius_(radius), grain_(ofs) {
    }

    Image Apply(const Image& image) override;

private:
    static constexpr int Permutation[] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142,
        8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203,
        117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
        71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,
        55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,
        18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250,
        124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,
        129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,
        242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
        181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114,
        67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,
        201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
        6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146,
        158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,
        65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188,
        159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248,
        152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110,
        79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179,
        162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176,
        115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195,
        78,  66,  215, 61,  156, 180};

    double radius_;

    double grain_;

private:
    double Noise(double x, double y) const;
};

struct GlassDistortionFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        double radius = command_line::Read<double>(argsstream);
        double ofs = command_line::Read<double>(argsstream);
        return std::make_unique<GlassDistortion>(radius, ofs);
    }
};

namespace {

const RegisterFilter<GrayscaleFactory> REG_GS("-gs");
const RegisterFilter<CropFactory> REG_CROP("-crop");
const RegisterFilter<NegativeFactory> REG_NEG("-neg");
const RegisterFilter<EdgeDetectionFactory> REG_EDGE("-edge");
const RegisterFilter<SharpeningFactory> REG_SHAPR("-sharp");
const RegisterFilter<GaussFactory> REG_BLUR("-blur");
const RegisterFilter<GlassDistortionFactory> REG_GLASS("-distort");

}  // namespace

}  // namespace image_processor::filters