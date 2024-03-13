#include "Filters/Filters.h"
#include <cstdint>
#include <iterator>

#include "Image/Image.h"
#include "Image/Pixel.h"

namespace image_processor::filters {

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

}  // namespace image_processor::filters