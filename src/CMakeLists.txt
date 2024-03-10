add_executable(cmaketest1 
    main.cpp
    ImageHandler/ImageHandler.cpp
)
target_include_directories(cmaketest1 PRIVATE .)
target_include_directories(cmaketest1 PUBLIC ../include)
target_compile_features(cmaketest1 PRIVATE cxx_std_20)
set_target_properties(cmaketest1 PROPERTIES CXX_EXTENSIONS OFF)