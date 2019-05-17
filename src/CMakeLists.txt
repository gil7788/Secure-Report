cmake_minimum_required (VERSION 3.0)

# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

file(GLOB SOURCE_FILES *.cpp)

# main
add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

# helib_test
add_executable(test_helib ../cpp_sketch/sketch_encoder.cpp test_helib.cc test_framework.h matrix.h)
target_link_libraries(test_helib Eigen3::Eigen liphe fhe ntl gmp pthread)

# zp_test
add_executable(test_zp ../cpp_sketch/sketch_encoder test_zp.cc test_framework.h matrix.h)
target_link_libraries(test_zp Eigen3::Eigen liphe fhe ntl gmp pthread)
