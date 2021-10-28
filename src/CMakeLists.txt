cmake_minimum_required (VERSION 3.0)
project(Secure)
# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set (CMAKE_CXX_STANDARD 14)
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

add_executable(run Run.cpp SketchEncoder.cpp SketchEncoder.h utils/mathmatical/VectorUtils.cpp utils/mathmatical/VectorUtils.h FHEDatabase.h utils/file_system/InputOutput.cpp utils/file_system/InputOutput.h Config.h FHEDatabaseConfig.cpp FHEDatabaseConfig.h Server.h Client.h FHEUtils.h TrustedThirdParty.cpp TrustedThirdParty.h PlainFileSystemDatabase.h GenericZP.h GenericZP.cpp SimplifiedHelibNumber.h SimplifiedHelibNumber.cpp SimplifiedHelibKeys.cpp EncryptedBits.h Queries.h HashFunctions/HashFunctionsFamily.cpp HashFunctions/HashFunctionsFamily.h Protocol.h utils/time/Duration.cpp utils/time/Duration.h utils/test/data/SecureRetrievalData.h  utils/test/test_case/SecureRetrievalTestCase.h utils/test/tester/ProtocolTester.cpp utils/test/tester/ProtocolTester.h utils/test/JsonLogger.cpp utils/test/JsonLogger.h utils/test/test_case/HashTestCase.cpp utils/test/test_case/HashTestCase.h utils/test/data/Data.cpp utils/test/data/Data.h utils/test/tester/Tester.cpp utils/test/tester/Tester.h utils/test/test_case/TestCase.h utils/test/data/HashFunctionData.cpp utils/test/data/HashFunctionData.h utils/test/data/TestResult.h utils/JsonParser.h utils/JsonParser.cpp)
target_link_libraries(run Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)

add_executable(test_vandermonde vandermonde_matrix/vandermonde_test.cpp vandermonde_matrix/vandermonde.cpp vandermonde_matrix/vandermonde.hpp HashFunctions/HashFunctionsFamily.h HashFunctions/HashFunctionsFamily.cpp utils/mathmatical/VectorUtils.cpp utils/mathmatical/VectorUtils.h utils/test/tester/TestHashFunctionFamily.cpp utils/test/tester/TestHashFunctionFamily.h Config.h utils/file_system/InputOutput.cpp utils/file_system/InputOutput.h utils/test/data/HashFunctionData.cpp utils/test/data/HashFunctionData.h utils/test/JsonLogger.cpp utils/test/JsonLogger.h utils/test/test_case/HashTestCase.cpp utils/test/test_case/HashTestCase.h utils/test/data/Data.cpp utils/test/data/Data.h utils/test/tester/Tester.cpp utils/test/tester/Tester.h utils/test/test_case/TestCase.h)
target_link_libraries(test_vandermonde Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)
