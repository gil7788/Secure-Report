cmake_minimum_required (VERSION 3.0)

file(GLOB SOURCE_FILES *.cpp)
#set(LIBS "liphe fhe ntl gmp pthread")

# main
add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

# helib_test
add_executable(test_helib test_helib.cc test_framework.h matrix.h sketch_matrix.cpp)
target_link_libraries(test_helib liphe fhe ntl gmp pthread)

# zp_test
add_executable(test_zp test_zp.cc test_framework.h matrix.h)
target_link_libraries(test_zp liphe fhe ntl gmp pthread)
