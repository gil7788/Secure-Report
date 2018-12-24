project(secure-report)
file(GLOB SOURCE_FILES *.cpp)

add_executable(test001 ${SOURCE_FILES})

target_link_libraries(test001 gtest pthread)
