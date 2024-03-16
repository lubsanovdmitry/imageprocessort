add_executable(test_clamped_vector test_clamped_vector.cpp)
target_include_directories(test_clamped_vector PRIVATE ../src)
target_link_libraries(test_clamped_vector PRIVATE Catch2::Catch2WithMain)

add_executable(iotest iotest.cpp ../src/IO/FileReader.cpp ../src/IO/FileWriter.cpp)
target_include_directories(iotest PRIVATE ../src)
target_compile_features(iotest PRIVATE cxx_std_20)
target_link_libraries(iotest PRIVATE Catch2::Catch2WithMain)