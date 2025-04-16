add_executable(image_processor 
        src/main.cpp
        src/ImageFormats/BMP.cpp
        src/IO/FileReader.cpp
        src/IO/FileWriter.cpp
        src/Image/Pixel.cpp
        src/Filters/Filters.cpp
        src/FilterController/FilterController.cpp
        src/CLI/CommandLineArgs.cpp
)
target_include_directories(image_processor PRIVATE src)
target_compile_features(image_processor PRIVATE cxx_std_20)

# add_subdirectory(tests)