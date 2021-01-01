cmake_minimum_required (VERSION 3.0)
project(Secure)
# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set (CMAKE_CXX_STANDARD 11)
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
include_directories(${MYSQL_INCLUDE_DIR} ${MYSQLPP_INCLUDE_DIR} ${LIPHE_INCLUDE_DIR})
link_directories(${MYSQLPP_LIBRARY})

file(GLOB SOURCE_FILES *.cpp)

add_executable(run Run.cpp SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h PlainDatabase.cpp PlainDatabase.h InputOutput.cpp InputOutput.h Config.h FHEDatabaseConfig.cpp FHEDatabaseConfig.h Server.h Client.h FHEUtils.h TrustedThirdParty.cpp TrustedThirdParty.h FileUtils.cpp FileUtils.h PlainFileSystemDatabase.h GenericZP.h GenericZP.cpp SimplifiedHelibNumber.h SimplifiedHelibNumber.cpp SimplifiedHelibKeys.cpp EncryptedBits.h Queries.h HashFunctions/HashFunctionsFamily.cpp HashFunctions/HashFunctionsFamily.h Protocol.h)
target_link_libraries(run Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)

add_executable(test_vandermonde vandermonde_matrix/vandermonde_test.cpp vandermonde_matrix/vandermonde.cpp vandermonde_matrix/vandermonde.hpp HashFunctions/HashFunctionsFamily.h HashFunctions/HashFunctionsFamily.cpp VectorUtils.cpp VectorUtils.h HashFunctions/TestHashFunctionFamily.cpp HashFunctions/TestHashFunctionFamily.h Config.h InputOutput.cpp InputOutput.h HashFunctions/HashFunctionData.cpp HashFunctions/HashFunctionData.h HashFunctions/HashFunctionLogData.cpp HashFunctions/HashFunctionLogData.h HashFunctions/SBRTestData.cpp HashFunctions/SBRTestData.h)
target_link_libraries(test_vandermonde Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)
