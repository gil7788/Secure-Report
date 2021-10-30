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


#add_executable(test001 ${SOURCE_FILES})
#target_link_libraries(test001 gtest pthread)
add_executable(test_encrypted_io ${SOURCE_FILES} test_read_write_encrypted_number.cpp ../src/infrastructure/file_system/InputOutput.cpp)
target_link_libraries(test_encrypted_io Eigen3::Eigen gtest pthread stdc++fs)
add_executable(helib_test ${SOURCE_FILES} test_read_write_encrypted_number.cpp ../src/algorithms/fully_homomorphic/SimplifiedHelibNumber.cpp ../src/algorithms/fully_homomorphic/GenericZP.cpp ../src/algorithms/fully_homomorphic/Context.cpp ../src/algorithms/fully_homomorphic/Context.h ../src/infrastructure/file_system/InputOutput.cpp ../src/infrastructure/VectorUtils.cpp HelibTest.cpp HelibTest.h ../src/algorithms/fully_homomorphic/SimplifiedHelibKeys.cpp ../src/algorithms/fully_homomorphic/SimplifiedHelibKeys.h)
target_link_libraries(helib_test Eigen3::Eigen liphe fhe ntl gmp pthread gtest pthread stdc++fs)
