#include "Image/Pixel.h"

namespace image_processor {

Pixel operator+(const Pixel& a, const Pixel& b) {
    Pixel p{};
    p.blue = a.blue + b.blue;
    p.green = a.green + b.green;
    p.red = a.red + b.red;
    return p;
}

Pixel operator-(const Pixel& a, const Pixel& b) {
    Pixel p{};
    p.blue = a.blue - b.blue;
    p.green = a.green - b.green;
    p.red = a.red - b.red;
    return p;
}

Pixel operator*(const Pixel& a, const Pixel& b) {
    Pixel p{};
    p.blue = a.blue * b.blue;
    p.green = a.green * b.green;
    p.red = a.red * b.red;
    return p;
}

Pixel operator*(const Pixel& c, double a) {
    Pixel p{};
    p.blue = c.blue * a;
    p.green = c.green * a;
    p.red = c.red * a;
    return p;
}
Pixel operator*(double a, const Pixel& c) {
    Pixel p{};
    p.blue = c.blue * a;
    p.green = c.green * a;
    p.red = c.red * a;
    return p;
}

Pixel operator/(const Pixel& c, double a) {
    Pixel p{};
    p.blue = c.blue / a;
    p.green = c.green / a;
    p.red = c.red / a;
    return p;
}

}  // namespace image_processor
