#pragma once

#include <cstddef>
#include <memory>
#include <sstream>

#include "Filters/BaseFilter.h"
#include "Filters/Factory.h"
#include "CLI/CommandLineArgs.h"

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
};

struct GaussFactory : public BaseFilterFactory {
    std::unique_ptr<BaseFilter> operator()(std::stringstream& argsstream) const override {
        double threshold = command_line::Read<double>(argsstream);
        return std::make_unique<Gaussian>(threshold);
    }
};

namespace {

const RegisterFilter<GrayscaleFactory> REG_GS("-gs");
const RegisterFilter<CropFactory> REG_CROP("-crop");
const RegisterFilter<NegativeFactory> REG_NEG("-neg");
const RegisterFilter<EdgeDetectionFactory> REG_EDGE("-edge");
const RegisterFilter<SharpeningFactory> REG_SHAPR("-sharp");
const RegisterFilter<GaussFactory> REG_BLUR("-blur");

}  // namespace

}  // namespace image_processor::filters