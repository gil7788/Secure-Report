cmake_minimum_required (VERSION 3.0)

# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

file(GLOB SOURCE_FILES *.cpp)

# helib_test
add_executable(test_helib test_helib.cc SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h)
target_link_libraries(test_helib Eigen3::Eigen liphe fhe ntl gmp pthread)

# zp_test
add_executable(test_zp test_zp.cc SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h)
target_link_libraries(test_zp Eigen3::Eigen liphe fhe ntl gmp pthread)
