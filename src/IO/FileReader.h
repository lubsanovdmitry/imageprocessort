#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <filesystem>
#include <fstream>
#include <ios>

namespace image_processor::io {

class FileReader {
public:
    FileReader() = default;

    explicit FileReader(std::filesystem::path path);

    FileReader& operator=(std::filesystem::path path);

    template <typename T>
    FileReader& operator>>(T& t);

    using PosType = typename std::ifstream::pos_type;

    using OffType = typename std::ifstream::off_type;

    FileReader& SeekGet(PosType pos);

    FileReader& SeekGet(OffType off, std::ios_base::seekdir);

private:
    std::ifstream in_;

    constexpr static bool IsBigEndian();

    template <typename T>
    T Read();
};

template <typename T>
T FileReader::Read() {
    std::array<char, sizeof(T)> buffer;
    in_.read(buffer.data(), buffer.size());

    if constexpr (std::endian::native == std::endian::big) {
        std::reverse(buffer.begin(), buffer.end());
    }

    return std::bit_cast<T>(buffer);
}

template <typename T>
FileReader& FileReader::operator>>(T& t) {
    t = Read<T>();
    return *this;
}

}  // namespace image_processor::io
