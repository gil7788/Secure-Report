cmake_minimum_required (VERSION 3.0)

# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# Set MySQL header files path
set(MYSQL_INCLUDE_DIR /usr/local/include/mysql)
# Set MySQLCpp header/cpp files path
set(MYSQLPP_INCLUDE_DIR /usr/local/include/mysql++)
set(MYSQLPP_LIBRARY /usr/local/lib/libmysqlpp)
# Include/link MySQL/Cpp
include_directories(${MYSQL_INCLUDE_DIR} ${MYSQLPP_INCLUDE_DIR})
link_directories(${MYSQLPP_LIBRARY})

file(GLOB SOURCE_FILES *.cpp)

# helib_test
add_executable(test_helib test_helib.cc SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h PlainDatabase.cpp PlainDatabase.h FHEUtils.h ctxt_heap.h)
target_link_libraries(test_helib Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp)

# zp_test
add_executable(test_zp test_zp.cc SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h PlainDatabase.cpp PlainDatabase.h FHEUtils.h ctxt_heap.h)
target_link_libraries(test_zp Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp)
