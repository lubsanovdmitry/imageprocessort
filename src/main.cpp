#include <iostream>
#include <vector>

#include "Utils/ClampedVector.h"
#include "Image/Image.h"
#include "ImageFormats/BMP.h"

int main(int argc, char** argv) {
    image_processor::BMP bmp;
    image_processor::Image image = bmp.Read("flag.bmp");
    bmp.Write(image, "flag2.bmp");
    return 0;
}
