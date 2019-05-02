cmake_minimum_required (VERSION 3.0)

# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

file(GLOB SOURCE_FILES *.cpp)
#set(LIBS "liphe fhe ntl gmp pthread")

# main
add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

# helib_test
add_executable(test_helib test_helib.cc test_framework.h matrix.h sketch_matrix.cpp sketch_matrix.h)
target_link_libraries(test_helib Eigen3::Eigen liphe fhe ntl gmp pthread)

# zp_test
add_executable(test_zp test_zp.cc test_framework.h matrix.h)
target_link_libraries(test_zp liphe fhe ntl gmp pthread)
