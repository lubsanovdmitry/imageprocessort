#pragma once

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
