#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>

class FileWriter {
public:
    explicit FileWriter(std::filesystem::path path);

    template <typename T>
    FileWriter& operator<<(T t);

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
    std::array<char, sizeof(T)> buffer;
    std::memcpy(buffer.data(), reinterpret_cast<T*>(&t), sizeof(T));

    if constexpr (std::endian::native == std::endian::big) {
        std::reverse(buffer.begin(), buffer.end());
    }

    out_.write(buffer.data(), sizeof(T));
}
