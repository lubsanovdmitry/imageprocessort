#pragma once

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>

class FileReader {
public:
    explicit FileReader(std::filesystem::path path);

    template <typename T>
    FileReader& operator>>(T& t);

private:
    std::ifstream in_;

    constexpr static bool IsBigEndian();

    template <typename T>
    T Read();
};

template <typename T>
T FileReader::Read() {
    std::array<char, sizeof(T)> buffer;
    in_.read(buffer.data(), sizeof(T));

    if constexpr (std::endian::native == std::endian::big) {
        std::reverse(buffer.begin(), buffer.end());
    }

    return *reinterpret_cast<char*>(buffer.data());
}

template <typename T>
FileReader& FileReader::operator>>(T& t) {
    t = Read<T>();
    return *this;
}
