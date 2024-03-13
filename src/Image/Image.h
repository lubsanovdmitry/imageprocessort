#pragma once

#include "Image/Pixel.h"
#include "Utils/ClampedVector.h"

namespace image_processor {

using Row = image_processor::ClampedVector<image_processor::Pixel>;
using Image = image_processor::ClampedVector<Row>;

}  // namespace image_processor