#include <catch2/catch_test_macros.hpp>

#include "IO/FileWriter.h"
#include "IO/FileReader.h"

TEST_CASE("sus") {
    image_processor::io::FileReader fr("test.dat");
    image_processor::io::FileWriter fw("write.dat");
    int64_t a{};
    fr >> a;
    fw << a;
}