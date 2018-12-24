file(GLOB SOURCE_FILES *.cpp)
#set(LIBS "liphe fhe ntl gmp pthread")

add_executable(main main.cpp)
target_link_libraries(main liphe fhe ntl gmp pthread)

add_executable(test_helib test_helib.cc)
target_link_libraries(test_helib fhe ntl gmp pthread liphe)

#add_executable(test_zp test_helib.cc)
#target_link_libraries(test_zp liphe)