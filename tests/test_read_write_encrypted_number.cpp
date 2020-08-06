//
// Created by gil on 6.8.2020.
//

#include "gtest/gtest.h"

#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEUtils.h"
#include "InputOutput.h"
#include "Config.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

int test(int x, int y) {
    MyZP encrypted_x = MyZP::static_from_int(x);
    MyZP encrypted_y = MyZP::static_from_int(y);
    MyZP encrypted_test_result = FHEUtils<MyZP>::areEqualBinary(encrypted_x, encrypted_y, 1);
    int test = encrypted_test_result.to_int();
    return test;
}

int test(MyZP x, MyZP y) {
    MyZP encrypted_test_result = FHEUtils<MyZP>::areEqualBinary(x, y, 1);
    int test = encrypted_test_result.to_int();
    return test;
}

MyZP file_to_number(std::string& file_path) {
    MyZP number;
    std::filebuf fb;
    if (! fb.open (file_path,std::ios::in)) {
        std::cout << "Failed to read from database \n";
    }
    else {
        std::istream is(&fb);
        is >> number;
    }
    return number;
}

bool number_to_file(MyZP& number, std::string& file_path) {
    InputOutput io = InputOutput(false, file_path, constants::OUTPUT_LEVEL);
    stringstream stringified_value;
    stringified_value << number;
    bool saved_to_file_system = io.output(stringified_value.str(), constants::ERROR);
    if(! saved_to_file_system) {
        cout << "Failed to insert number " << number << ".\n";
        return false;
    }
    return true;
}

MyZP save_and_read(MyZP& number, std::string& file_path) {
    bool saved = number_to_file(number, file_path);
    assert(saved);
    MyZP encrypted_from_file = file_to_number(file_path);
    return encrypted_from_file;
}

TEST(test_is_match_eq, zero_zero) {
    int result = test(0, 0);
    EXPECT_EQ(result, 1);
}

TEST(test_is_match_eq, zero_one) {
    int result = test(0, 1);
    EXPECT_EQ(result, 0);
}

TEST(test_is_match_eq, one_zero) {
    int result = test(1, 0);
    EXPECT_EQ(result, 0);
}

TEST(test_is_match_eq, one_one) {
    int result = test(1, 1);
    EXPECT_EQ(result, 1);
}

TEST(test_is_match_eq_read_from_file_system, zero_zero) {
    MyZP encrypted_zero = MyZP::static_from_int(0);
    MyZP encrypted_one = MyZP::static_from_int(1);
    std::string file_path_one = "encrypted_one";
    std::string file_path_zero = "encrypted_zero";

    auto encrypted_zero_from_file = save_and_read(encrypted_zero, file_path_zero);

    int result = test(encrypted_zero, encrypted_zero_from_file);
    EXPECT_EQ(result, 1);
}

TEST(test_is_match_eq_read_from_file_system, zero_one) {
    MyZP encrypted_zero = MyZP::static_from_int(0);
    MyZP encrypted_one = MyZP::static_from_int(1);
    std::string file_path_one = "encrypted_one";
    std::string file_path_zero = "encrypted_zero";

    auto encrypted_one_from_file = save_and_read(encrypted_one, file_path_one);

    int result = test(encrypted_zero, encrypted_one_from_file);
    EXPECT_EQ(result, 0);
}

TEST(test_is_match_eq_read_from_file_system, one_zero) {
    MyZP encrypted_zero = MyZP::static_from_int(0);
    MyZP encrypted_one = MyZP::static_from_int(1);
    std::string file_path_one = "encrypted_one";
    std::string file_path_zero = "encrypted_zero";

    auto encrypted_zero_from_file = save_and_read(encrypted_zero, file_path_zero);

    int result = test(encrypted_one, encrypted_zero_from_file);
    EXPECT_EQ(result, 0);
}

TEST(test_is_match_eq_read_from_file_system, one_one) {
    MyZP encrypted_one = MyZP::static_from_int(1);
    std::string file_path_one = "encrypted_one";
    std::string file_path_zero = "encrypted_zero";

    auto encrypted_one_from_file = save_and_read(encrypted_one, file_path_one);

    int result = test(encrypted_one, encrypted_one_from_file);
    EXPECT_EQ(result, 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}