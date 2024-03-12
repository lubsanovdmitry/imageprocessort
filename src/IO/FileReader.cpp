#include "IO/FileReader.h"

#include <algorithm>

FileReader::FileReader(std::filesystem::path path) : in_(path, std::ios::binary) {
    if (!in_.is_open()) {
        throw std::runtime_error{"Could not open file for reading."};
    }
}

constexpr bool FileReader::IsBigEndian() {
    if constexpr (std::endian::native == std::endian::big) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
T FileReader::Read() {
    char* buffer[sizeof(T)];
    in_.read(buffer, sizeof(T));
    return *reinterpret_cast<char*>(buffer);
}

template <typename T>
FileReader& FileReader::operator>>(T& t) {
    t = Read<T>();
    return *this;
}
