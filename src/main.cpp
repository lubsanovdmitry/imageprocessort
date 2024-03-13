#include <iostream>
#include <vector>

#include <catch2/catch_all.hpp>

#include "Utils/ClampedVector.h"
#include "Image/Image.h"
#include "ImageFormats/BMP.h"
#include "Filters/Filters.h"

int main(int argc, char** argv) {
    image_processor::BMP bmp;
    image_processor::Image image = bmp.Read("flag6.bmp");
    image_processor::filters::Grayscale gs;
    auto i = gs.Apply(image);
    bmp.Write(i, "flag2.bmp");
    return 0;
}
