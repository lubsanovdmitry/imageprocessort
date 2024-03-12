#include "IO/FileWriter.h"

FileWriter::FileWriter(std::filesystem::path path) : out_(path, std::ios::binary) {
    if (!out_.is_open()) {
        throw std::runtime_error{"Could not open file for writing."};
    }
}

template <typename T>
FileWriter& FileWriter::operator<<(T t) {
    Write(t);
    return *this;
}

template <typename T>
void FileWriter::Write(T t) {
    char* buffer[sizeof(T)] = reinterpret_cast<char*>(&t);
    out_.write(buffer, sizeof(T));
}
