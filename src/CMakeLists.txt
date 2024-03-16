target_sources(image_processor PRIVATE
        main.cpp
        ImageFormats/BMP.cpp
        IO/FileReader.cpp
        IO/FileWriter.cpp
        Image/Image.cpp
        Image/Pixel.cpp
        Filters/Filters.cpp
        FilterController/FilterController.cpp
        CLI/CommandLineArgs.cpp
)
target_include_directories(image_processor PRIVATE .)
target_compile_features(image_processor PRIVATE cxx_std_20)