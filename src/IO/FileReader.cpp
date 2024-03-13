#include "IO/FileReader.h"

FileReader::FileReader(std::filesystem::path path) : in_(path, std::ios::binary) {
    if (!in_.is_open()) {
        throw std::runtime_error{"Could not open file for reading."};
    }
}
