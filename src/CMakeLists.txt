file(GLOB SOURCE_FILES *.cpp)
#set(LIBS "liphe fhe ntl gmp pthread")

add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(call_python call_python.cpp)
target_link_libraries(call_python ${PYTHON_LIBRARIES} liphe fhe ntl gmp pthread)

add_executable(test_helib test_helib.cc test_framework.h matrix.h)
target_link_libraries(test_helib liphe fhe ntl gmp pthread)

add_executable(test_zp test_zp.cc test_framework.h matrix.h)
target_link_libraries(test_zp liphe fhe ntl gmp pthread)