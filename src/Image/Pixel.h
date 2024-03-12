#pragma once

namespace image {

struct Pixel {
    double blue = 0.0;
    double green = 0.0;
    double red = 0.0;
};

Pixel operator+(const Pixel& a, const Pixel& b);
Pixel operator-(const Pixel& a, const Pixel& b);
Pixel operator*(const Pixel& a, const Pixel& b);
Pixel operator*(const Pixel& c, double a);
Pixel operator*(double a, const Pixel& c);
Pixel operator/(const Pixel& c, double a);

}  // namespace image
