//
// Created by gil on 16.8.2020.
//

#include <NTL/vec_long.h>
#include <FHEDatabaseConfig.h>
#include "HelibTest.h"
#include "gtest/gtest.h"
#include "VectorUtils.h"
#include "SimplifiedHelibNumber.h"

using namespace std;

EncryptedDataTypeFromParameters init_helib() {
    int size = 128;
    long r = 2;
    int L = 7;
    int p = 2;

    long R = 1;
    long d = 1;
    long c = 2;
    long k = 80;
    long s = 0;
    long chosen_m = 0;
    Vec<long> gens;
    Vec<long> ords;
    const std::string key_file_path = "../../key";
    EncryptedDataTypeFromParameters encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);
    return encrypted_data_type;
}

int test_init_and_sum_binary(int number1, int number2) {
    auto data_type = init_helib();
    data_type.initialize();
    auto five_vector = VectorUtils::number_to_std_vector(number1, constants::WORD_LENGTH);
    auto six_vector = VectorUtils::number_to_std_vector(number2, constants::WORD_LENGTH);

    SimplifiedHelibNumber encrypted_five_vector(five_vector);
    SimplifiedHelibNumber encrypted_six_vector(six_vector);
    cout << "Number 1 vector: " << encrypted_five_vector.to_int() << "\n";
    cout << "Number 2 vector: " << encrypted_six_vector.to_int() << "\n";

    auto encrypted_result = encrypted_five_vector + encrypted_six_vector;
    auto result = encrypted_result.to_int();
    return result;
}

TEST (test_init_and_extract_bits_dirty, one) {
    vector<long> expected = {1, 0, 1};
    int number = 3;
    auto data_type = init_helib();
    data_type.initialize();
    auto encrypted_number = SimplifiedHelibNumber(number);
    vector<Ctxt> bits;
    vector<SimplifiedHelibNumber> bits_wrappers;
    extractDigits(bits, encrypted_number.v(), 0, false);
    for(auto& bit: bits) {
        SimplifiedHelibNumber wrapper(bit);
        bits_wrappers.push_back(wrapper);
    }

    for(auto& bit: bits_wrappers) {
        auto value = bit.to_int();
        cout << value << "\n";
    }

    auto encrypted_digits = encrypted_number.to_digits<Bits<SimplifiedHelibNumber>>();
    vector<long> digits;
    cout << "value: " << encrypted_number.to_int() << "\n";
    cout << "Binary representation: \n";
    for(int i = 0; i < encrypted_digits.size(); ++i) {
        digits.push_back(encrypted_digits[i].to_int());
        cout << encrypted_digits[i].to_int();
    }
    auto result = VectorUtils::std_vector_to_number(digits);
    auto expected_value = VectorUtils::std_vector_to_number(expected);
    EXPECT_EQ(0, expected_value);
}

TEST (test_init_and_extract_bits, one) {
    int number = 3;
    int expected = number;
    auto data_type = init_helib();
    data_type.initialize();
    auto encrypted_number = SimplifiedHelibNumber(number);

    auto encrypted_digits = encrypted_number.to_digits<Bits<SimplifiedHelibNumber>>();
    vector<long> digits;
    cout << "value: " << encrypted_number.to_int() << "\n";
    cout << "Binary representation: \n";
    for(int i = 0; i < encrypted_digits.size(); ++i) {
        digits.push_back(encrypted_digits[i].to_int());
        cout << digits[i];
    }
    auto result = VectorUtils::std_vector_to_number(digits);

    EXPECT_EQ(expected, result);
}

TEST(test_init_and_sum_binary, five_six) {
    int expected = 11;
    int result = test_init_and_sum_binary(5,6);
    EXPECT_EQ(result, expected);
}

TEST(test_init_and_sum_binary, zero_one) {
    int expected = 1;
    int result = test_init_and_sum_binary(0,1);
    EXPECT_EQ(result, expected);
}

TEST(test_init_and_sum_binary_overflow, one_hundred_two_hundred) {
    int expected = 172;
    int result = test_init_and_sum_binary(100,200);
    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}