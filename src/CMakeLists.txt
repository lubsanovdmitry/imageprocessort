add_executable(cmaketest1
        main.cpp
        ImageFormats/BMP.cpp
        IO/FileReader.cpp
        IO/FileWriter.cpp
        Image/Image.cpp
        Image/Pixel.cpp
)
target_include_directories(cmaketest1 PRIVATE .)
target_include_directories(cmaketest1 PUBLIC ../include)
target_compile_features(cmaketest1 PRIVATE cxx_std_20)
set_target_properties(cmaketest1 PROPERTIES CXX_EXTENSIONS OFF)