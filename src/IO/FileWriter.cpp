#include "IO/FileWriter.h"

namespace image_processor::io {

FileWriter::FileWriter(std::filesystem::path path) : out_(path, std::ios::binary) {
    if (!out_.is_open()) {
        throw std::runtime_error{"Could not open  " + path.string() + " for writing"};
    }
}

FileWriter& FileWriter::operator=(std::filesystem::path path) {
    out_.close();
    out_.open(path);
    if (!out_.is_open()) {
        throw std::runtime_error{"Could not open " + path.string() + " for writing"};
    }
    return *this;
}

FileWriter& FileWriter::SeekPut(PosType pos) {
    out_.seekp(pos);
    return *this;
}

FileWriter& FileWriter::SeekPut(OffType off, std::ios_base::seekdir dir) {
    out_.seekp(off, dir);
    return *this;
}

}  // namespace image_processor::io
