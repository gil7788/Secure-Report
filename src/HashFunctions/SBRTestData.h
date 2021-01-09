//
// Created by gil on 28.11.2020.
//

#ifndef SECURE_SBRTESTDATA_H
#define SECURE_SBRTESTDATA_H

#include <chrono>
using namespace std::chrono;

#include "HashFunctionsFamily.h"
#include "HashFunctionData.h"


class TestCase {
public:
    virtual ~TestCase() {};

    virtual string to_string() = 0;

    virtual unique_ptr<Data> test() = 0;
};

class HashFunctionTestCase: public TestCase {
    unique_ptr<HashFunctionFamily> _hash_function;
    int _database_size;
    int _number_of_matches;
    int _number_of_batches;
    double _average_batch_size;

public:
    HashFunctionTestCase(unique_ptr<HashFunctionFamily>& hash_function, int database_size, int number_of_matches, double average_batch_size);

    string to_string() override;

    unique_ptr<Data> test() override;

private:
    int get_number_of_batches(double average_batch_size, int number_of_matches);

    int get_word_length(int number);

    std::vector<int> sample_indices(int number_of_indices, int range_upper_bound);
};


// class SBRTestData {
// private:
//     HashFunctionFamily& _hash_function;
//     int _database_size;
//     int _number_of_matches;
//     int _number_of_batches;
//     double _average_batch_size;

// public:
//     SBRTestData(HashFunctionFamily& hash_function, int database_size, int number_of_matches, double average_batch_size);

//     void test(TestHashFunctionFamily& tester);

//     string to_string();

// private:
//     int get_number_of_batches(double number_of_batches, int number_of_matches);

//     int get_word_length(int number);
// };


#endif //SECURE_SBRTESTDATA_H
