#pragma once

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
