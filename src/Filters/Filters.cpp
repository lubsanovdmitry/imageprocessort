#include "Filters/Filters.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numbers>

#include "Filters/BaseFilter.h"
#include "Image/Image.h"
#include "Image/Pixel.h"

namespace image_processor::filters {

Image BaseConvolutionFilter::ApplyMatrix(const Image& image, const ConvolutionMatrix& cm) {
    Image new_image(image.Size(), Row(image[0].Size()));

    for (int64_t i = 0; i < static_cast<int64_t>(image.Size()); ++i) {
        for (int64_t j = 0; j < static_cast<int64_t>(image[i].Size()); ++j) {
            for (const Index& idx : IndexArray) {
                new_image[i][j].blue += cm[idx.x + 1][idx.y + 1] * image[i + idx.x][j + idx.y].blue;
                new_image[i][j].green += cm[idx.x + 1][idx.y + 1] * image[i + idx.x][j + idx.y].green;
                new_image[i][j].red += cm[idx.x + 1][idx.y + 1] * image[i + idx.x][j + idx.y].red;
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

    if (new_height_ > image.Size()) {
        new_height_ = image.Size();
    }

    if (!new_image.Empty() && new_image[0].Size() < new_width_) {
        new_width_ = new_image[0].Size();
    }

    for (Row& r : new_image) {
        r.Resize(new_width_);
    }

    int64_t new_start = static_cast<int64_t>(new_image.Size() - new_height_);

    new_image.Erase(new_image.begin(), std::next(new_image.begin(), new_start));

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
            if (p.blue > threshold_) {
                p = {1, 1, 1};
            } else {
                p = {0, 0, 0};
            }
        }
    }

    return new_image;
}

Image Gaussian::Apply(const Image& image) {
    Image new_image(image.Size(), Row(image[0].Size()));

    for (int64_t i = 0; i < image.Size(); ++i) {
        for (int64_t j = 0; j < image[0].Size(); ++j) {
            // std::cerr << i << ' ' << j << ' ';
            for (int64_t x = i - 3; x < i + 3; ++x) {
                for (int64_t y = j - 3; y < j + 3; ++y) {
                    double distx = static_cast<double>(std::abs(i - x));
                    double disty = static_cast<double>(std::abs(j - y));
                    double normdist = (distx * distx + disty * disty) / (2 * sigma_ * sigma_);
                    double expon = std::exp(-normdist);
                    double coeff = expon / (2 * std::numbers::pi * sigma_ * sigma_);
                    new_image[i][j].blue += coeff * image[x][y].blue;
                    new_image[i][j].green += coeff * image[x][y].green;
                    new_image[i][j].red += coeff * image[x][y].red;
                }
            }
            new_image[i][j].blue = std::clamp(new_image[i][j].blue, 0.0, 1.0);
            new_image[i][j].green = std::clamp(new_image[i][j].green, 0.0, 1.0);
            new_image[i][j].red = std::clamp(new_image[i][j].red, 0.0, 1.0);
        }
        // std::cerr << i << std::endl;
    }
    return new_image;
}

}  // namespace image_processor::filters