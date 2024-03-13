#include "IO/FileWriter.h"

FileWriter::FileWriter(std::filesystem::path path) : out_(path, std::ios::binary) {
    if (!out_.is_open()) {
        throw std::runtime_error{"Could not open file for writing."};
    }
}
