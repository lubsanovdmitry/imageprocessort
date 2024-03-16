#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <filesystem>
#include <fstream>

namespace image_processor::io {

class FileWriter {
public:
    FileWriter() = default;

    explicit FileWriter(std::filesystem::path path);

    FileWriter& operator=(std::filesystem::path path);

    template <typename T>
    FileWriter& operator<<(T t);

    using PosType = typename std::ofstream::pos_type;

    using OffType = typename std::ofstream::off_type;

    FileWriter& SeekPut(PosType pos);

    FileWriter& SeekPut(OffType off, std::ios_base::seekdir);

private:
    std::ofstream out_;

    template <typename T>
    void Write(T t);
};

template <typename T>
FileWriter& FileWriter::operator<<(T t) {
    Write(t);
    return *this;
}

template <typename T>
void FileWriter::Write(T t) {
    std::array<char, sizeof(T)> buffer = std::bit_cast<std::array<char, sizeof(T)>>(t);

    if constexpr (std::endian::native == std::endian::big) {
        std::reverse(buffer.begin(), buffer.end());
    }

    out_.write(buffer.data(), sizeof(T));
}

}  // namespace image_processor::io
