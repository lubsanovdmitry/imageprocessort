#include "IO/FileReader.h"

namespace image_processor::io {

FileReader::FileReader(std::filesystem::path path) : in_(path, std::ios::binary) {
    if (!in_.is_open()) {
        throw std::runtime_error{"Could not open " + path.string() + " for reading"};
    }
}

FileReader& FileReader::operator=(std::filesystem::path path) {
    in_.close();
    in_.open(path);
    if (!in_.is_open()) {
        throw std::runtime_error{"Could not open " + path.string() + " for reading"};
    }
    return *this;
}

FileReader& FileReader::SeekGet(PosType pos) {
    in_.seekg(pos);
    return *this;
}

FileReader& FileReader::SeekGet(OffType off, std::ios_base::seekdir dir) {
    in_.seekg(off, dir);
    return *this;
}

}  // namespace image_processor::io
