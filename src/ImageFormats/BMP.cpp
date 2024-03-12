#include "ImageFormats/BMP.h"

#include "IO/FileReader.h"
#include "IO/FileWriter.h"

void BMP::Read(std::filesystem::path path) {
    FileReader reader(path);
    reader >>
}