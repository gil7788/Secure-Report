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
include_directories(${MYSQL_INCLUDE_DIR} ${MYSQLPP_INCLUDE_DIR} ${LIPHE_INCLUDE_DIR})
link_directories(${MYSQLPP_LIBRARY})

file(GLOB SOURCE_FILES *.cpp)

add_executable(run Run.cpp SketchEncoder.cpp SketchEncoder.h VectorUtils.cpp VectorUtils.h FHEDatabase.h PlainDatabase.cpp PlainDatabase.h InputOutput.cpp InputOutput.h Config.h FHEDatabaseConfig.cpp FHEDatabaseConfig.h Server.cpp Server.cpp Server.h Client.h FHEUtils.h TrustedThirdParty.cpp TrustedThirdParty.h templates_test.cpp FileUtils.cpp FileUtils.h)
target_link_libraries(run Eigen3::Eigen liphe fhe ntl gmp pthread mysqlpp stdc++fs)
