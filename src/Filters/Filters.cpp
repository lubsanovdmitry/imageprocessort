#include "Filters/Filters.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numbers>

#include "Filters/BaseFilter.h"
#include "Image/Image.h"
#include "Image/Pixel.h"

namespace image_processor::filters {

Image BaseConvolutionFilter::ApplyMatrix(const Image& image, const ConvolutionMatrix& cm) {
    Image new_image(image.Size(), Row(image[0].Size()));
    // static_assert(cm.size() % 2 == 1, "a");
    // Image new_image = image;
    int64_t mid_x = static_cast<int64_t>(cm.size()) / 2;
    int64_t mid_y = static_cast<int64_t>(cm[0].size()) / 2;

    int64_t min_x = -mid_x;
    int64_t min_y = -mid_y;
    int64_t max_x = mid_x;
    int64_t max_y = mid_y;
    for (int64_t i = 0; i < static_cast<int64_t>(image.Size()); ++i) {
        for (int64_t j = 0; j < static_cast<int64_t>(image[i].Size()); ++j) {
            for (int64_t x = min_x; x <= max_x; ++x) {
                for (int64_t y = min_y; y <= max_y; ++y) {
                    new_image[i][j].blue += cm[x + mid_x][y + mid_y] * image[i + x][j + y].blue;
                    new_image[i][j].green += cm[x + mid_x][y + mid_y] * image[i + x][j + y].green;
                    new_image[i][j].red += cm[x + mid_x][y + mid_y] * image[i + x][j + y].red;
                }
            }
            new_image[i][j].blue = std::clamp(new_image[i][j].blue, 0.0, 1.0);
            new_image[i][j].green = std::clamp(new_image[i][j].green, 0.0, 1.0);
            new_image[i][j].red = std::clamp(new_image[i][j].red, 0.0, 1.0);
        }
    }

    return new_image;
}

Image Crop::Apply(const Image& image) {
    Image new_image = image;

    if (new_width_ > image.Size()) {
        new_width_ = image.Size();
    }

    if (!new_image.Empty() && new_image[0].Size() < new_height_) {
        new_height_ = new_image[0].Size();
    }

    for (Row& r : new_image) {
        r.Resize(new_width_);
    }

    // int64_t new_start = static_cast<int64_t>(new_image.Size() - new_height_);

    // new_image.Erase(new_image.begin(), std::next(new_image.begin(), new_start));

    new_image.Resize(new_height_);

    return new_image;
}

Image Grayscale::Apply(const Image& image) {
    Image new_image = image;

    for (Row& r : new_image) {
        for (Pixel& p : r) {
            Pixel old{p};
            p.blue = p.green = p.red = Red * old.red + Green * old.green + Blue * old.blue;
        }
    }

    return new_image;
}

Image Negative::Apply(const Image& image) {
    Image new_image = image;

    for (Row& r : new_image) {
        for (Pixel& p : r) {
            p.blue = 1.0 - p.blue;
            p.green = 1.0 - p.green;
            p.red = 1.0 - p.red;
        }
    }

    return new_image;
}

Image Sharpening::Apply(const Image& image) {
    Image new_image = ApplyMatrix(image, ConvMatrix);
    return new_image;
}

Image EdgeDetection::Apply(const Image& image) {
    Grayscale gs;
    Image new_image = ApplyMatrix(gs.Apply(image), ConvMatrix);

    for (Row& r : new_image) {
        for (Pixel& p : r) {
            if (p.blue >= threshold_) {
                p = {1, 1, 1};
            } else {
                p = {0, 0, 0};
            }
        }
    }

    return new_image;
}

Image Gaussian::Apply(const Image& image) {
    Image new_image_y_blur(image.Size(), Row(image[0].Size()));
    Image new_image(image.Size(), Row(image[0].Size()));

    std::array<double, Radius> gaussian_1d{};

    [[maybe_unused]] double sum = 0.0;

    int64_t mid = Radius / 2;
    int64_t min_x = -mid;
    int64_t max_x = mid;

    for (int64_t x = min_x; x <= max_x; ++x) {
        gaussian_1d[x + mid] = GaussDistrib(static_cast<double>(x));
        sum += gaussian_1d[x + mid];
    }

    for (int64_t x = min_x; x <= max_x; ++x) {
        gaussian_1d[x + mid] /= sum;
    }

    for (int64_t i = 0; i < image.Size(); ++i) {
        for (int64_t j = 0; j < image[0].Size(); ++j) {
            for (int64_t x = min_x; x <= max_x; ++x) {
                new_image_y_blur[i][j] = new_image_y_blur[i][j] + image[i][j + x] * gaussian_1d[x + mid];
            }
        }
    }
    for (int64_t i = 0; i < image.Size(); ++i) {
        for (int64_t j = 0; j < image[0].Size(); ++j) {
            for (int64_t x = min_x; x <= max_x; ++x) {
                new_image[i][j] = new_image[i][j] + new_image_y_blur[i + x][j] * gaussian_1d[x + mid];
            }
        }
    }

    return new_image;
}

double Gaussian::GaussDistrib(double x) const {
    double invrt = std::sqrt(1 / (2 * std::numbers::pi * sigma_ * sigma_));
    double expn = std::exp(-(x * x) / (2 * sigma_ * sigma_));
    return invrt * expn;
}

Image GlassDistortion::Apply(const Image& image) {
    Image new_image(image.Size(), Row(image[0].Size()));

    for (int64_t i = 0; i < image.Size(); ++i) {
        for (int64_t j = 0; j < image[0].Size(); ++j) {
            double xf = static_cast<double>(i) / (static_cast<double>(image.Size()) / grain_);
            double xoff = static_cast<double>(i + 1) / (static_cast<double>(image.Size()) / grain_);
            double yf = static_cast<double>(j) / (static_cast<double>(image[0].Size()) / grain_);
            double yoff = static_cast<double>(j + 1) / (static_cast<double>(image[0].Size()) / grain_);

            double n0 = Noise(xf, yf);
            double n1 = Noise(xoff, yf);
            double n2 = Noise(xf, yoff);

            int64_t dx = static_cast<int64_t>((n1 - n0) * 8 * radius_ + 0.5);  // NOLINT
            int64_t dy = static_cast<int64_t>((n2 - n0) * 8 * radius_ + 0.5);  // NOLINT

            new_image[i][j] = image[i + dx][j + dy];
        }
    }

    return new_image;
}

double GlassDistortion::Noise(double x, double y) const {
    constexpr int MaxCoord = 255;

    double ix{};
    double iy{};

    double fx = std::modf(x, &ix);
    double fy = std::modf(y, &iy);

    int hx = static_cast<int>(ix) & MaxCoord;
    int hy = static_cast<int>(iy) & MaxCoord;

    auto fade = [](double x) {
        return 6 * std::pow(x, 5) - 15 * std::pow(x, 4) + 10 * std::pow(x, 3);  // NOLINT
    };

    auto gradient = [](size_t c, double x, double y) {
        static constexpr std::array<std::array<double, 2>, 4> Vecs{{{{0, 1}}, {{0, -1}}, {{1, 0}}, {{-1, 0}}}};
        const auto& grad_co = Vecs[c % 4];
        return grad_co[0] * x + grad_co[1] * x;
    };

    double u = fade(fx);
    double v = fade(fy);
    double n00 = gradient(Permutation[Permutation[hx] + hy], fx, fy);
    double n01 = gradient(Permutation[Permutation[hx] + hy + 1], fx, fy - 1);
    double n11 = gradient(Permutation[Permutation[hx + 1] + hy + 1], fx - 1, fy - 1);
    double n10 = gradient(Permutation[Permutation[hx + 1] + hy], fx - 1, fy);

    double x1 = std::lerp(n00, n10, u);
    double x2 = std::lerp(n01, n11, u);
    return std::lerp(x1, x2, v);
}

}  // namespace image_processor::filters