#include "FilterController/FilterController.h"

namespace image_processor::filter_controller {

image_processor::Image FilterController::Apply(const image_processor::Image& image) {
    image_processor::Image new_image = image;
    for (const auto& i : filter_array_) {
        new_image = i->Apply(new_image);
    }
    return new_image;
}

}  // namespace image_processor::filter_controller