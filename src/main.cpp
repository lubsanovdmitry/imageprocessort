#include "ImageHandler/ImageHandler.h"

#include <iostream>

int main(int argc, char** argv) {
    ImageHandler image;
    image.Read("flag.bmp");
    std::cout << "kek" << std::endl;
    return 0;
}
