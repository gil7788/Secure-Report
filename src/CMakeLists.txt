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

add_executable(run Run.cpp
        algorithms/encoders/SketchEncoder.cpp
        algorithms/encoders/SketchEncoder.h
        infrastructure/VectorUtils.cpp
        infrastructure/VectorUtils.h
        infrastructure/file_system/FHEDatabase.h
        infrastructure/file_system/InputOutput.cpp
        infrastructure/file_system/InputOutput.h
        Config.h algorithms/fully_homomorphic/Context.cpp
        algorithms/fully_homomorphic/Context.h
        entities/Server.h
        entities/Client.h
        algorithms/fully_homomorphic/Comparators.h
        entities/TrustedThirdParty.cpp
        entities/TrustedThirdParty.h
        infrastructure/file_system/PlainFileSystemDatabase.h
        algorithms/fully_homomorphic/GenericZP.h
        algorithms/fully_homomorphic/GenericZP.cpp
        algorithms/fully_homomorphic/SimplifiedHelibNumber.h
        algorithms/fully_homomorphic/SimplifiedHelibNumber.cpp
        algorithms/fully_homomorphic/SimplifiedHelibKeys.cpp
        algorithms/fully_homomorphic/Bits.h entities/Queries.h
        algorithms/hash_functions/HashFunctionsFamily.cpp
        algorithms/hash_functions/HashFunctionsFamily.h
        entities/Protocol.h
        infrastructure/test/data/SecureRetrievalData.h
        infrastructure/test/test_case/SecureRetrievalTestCase.h
        infrastructure/test/tester/ProtocolTester.cpp
        infrastructure/test/tester/ProtocolTester.h
        infrastructure/file_system/JsonLogger.cpp
        infrastructure/file_system/JsonLogger.h
        infrastructure/test/test_case/HashTestCase.cpp
        infrastructure/test/test_case/HashTestCase.h
        infrastructure/test/data/Data.cpp
        infrastructure/test/data/Data.h
        infrastructure/test/tester/Tester.cpp
        infrastructure/test/tester/Tester.h
        infrastructure/test/test_case/TestCase.h
        infrastructure/test/data/HashFunctionData.cpp
        infrastructure/test/data/HashFunctionData.h
        infrastructure/test/data/TestResult.h
        infrastructure/JsonParser.h
        infrastructure/JsonParser.cpp)
target_link_libraries(run Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)

#add_executable(hash_run algorithms/hash_functions/HashFunctionsFamily.h
#        algorithms/hash_functions/HashFunctionsFamily.cpp
#        infrastructure/JsonParser.h
#        infrastructure/JsonParser.cpp
#        infrastructure/VectorUtils.cpp
#        infrastructure/VectorUtils.h
#        infrastructure/test/tester/HashFunctionFamilyTester.cpp
#        infrastructure/test/tester/HashFunctionFamilyTester.h
#        Config.h infrastructure/file_system/InputOutput.cpp
#        infrastructure/file_system/InputOutput.h
#        infrastructure/test/data/HashFunctionData.cpp
#        infrastructure/test/data/HashFunctionData.h
#        infrastructure/file_system/JsonLogger.cpp
#        infrastructure/file_system/JsonLogger.h
#        infrastructure/test/test_case/HashTestCase.cpp
#        infrastructure/test/test_case/HashTestCase.h
#        infrastructure/test/data/Data.cpp
#        infrastructure/test/data/Data.h
#        infrastructure/test/tester/Tester.cpp
#        infrastructure/test/tester/Tester.h
#        infrastructure/test/test_case/TestCase.h)
#target_link_libraries(hash_run Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)
