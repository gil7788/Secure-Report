project(secure-report)

cmake_minimum_required (VERSION 3.0)

# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(INCLUDE_DIR /usr/local/include)
set(LIBRARY_DIR usr/local/lib)

# Set MySQL header files path
set(MYSQL_INCLUDE_DIR ${INCLUDE_DIR}/mysql)

# Set MySQLCpp header/cpp files path
set(MYSQLPP_INCLUDE_DIR ${INCLUDE_DIR}/mysql++)
set(MYSQLPP_LIBRARY ${LIBRARY_DIR}/libmysqlpp)

# Set FHE header/cpp files path
set(FHE_INCLUDE_DIR ${INCLUDE_DIR}/FHE)
set(FHE_LIBRARY ${LIBRARY_DIR}/libliphe)

# Set LIPHE header/cpp files path
set(LIPHE_INCLUDE_DIR ${INCLUDE_DIR}/LIPHE)
set(LIPHE_LIBRARY ${LIBRARY_DIR}/libfhe)

# Include/link MySQL/Cpp
include_directories(${MYSQL_INCLUDE_DIR} ${MYSQLPP_INCLUDE_DIR} ${LIPHE_INCLUDE_DIR} ../src)
link_directories(${MYSQLPP_LIBRARY})

file(GLOB SOURCE_FILES *.cpp)

add_subdirectory(googletest-master)


add_executable(test001 ${SOURCE_FILES})
add_executable(test_encrypted_io ${SOURCE_FILES} test_read_write_encrypted_number.cpp ../src/InputOutput.cpp)
target_link_libraries(test001 gtest pthread)
target_link_libraries(test_encrypted_io Eigen3::Eigen gtest pthread stdc++fs)
