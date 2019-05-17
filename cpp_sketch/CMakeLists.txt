# Eigen package
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(saffron_example sketch_encoder.cpp saffron_example.cpp)
target_link_libraries(saffron_example Eigen3::Eigen)

add_executable(test_saffron sketch_encoder.cpp test_saffron.cpp)
target_link_libraries(test_saffron Eigen3::Eigen)