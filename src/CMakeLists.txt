file(GLOB SOURCE_FILES *.cpp)
#set(LIBS "liphe fhe ntl gmp pthread")

add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

add_executable(create_python_matrix python_api.cpp)
include_directories(/usr/include/python3.5m)
target_link_libraries(create_python_matrix python3.5m)


add_executable(test_helib test_helib.cc test_framework.h matrix.h python_api.cpp python_api.h)
target_link_libraries(test_helib liphe fhe ntl gmp pthread python3.5m)

add_executable(test_zp test_zp.cc test_framework.h matrix.h python_api.cpp python_api.h)
target_link_libraries(test_zp liphe fhe ntl gmp pthread python3.5m)