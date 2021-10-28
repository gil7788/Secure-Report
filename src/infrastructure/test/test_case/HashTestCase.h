//
// Created by gil on 28.11.2020.
//

#ifndef SECURE_HASHTESTCASE_H
#define SECURE_HASHTESTCASE_H

#include <chrono>
using namespace std::chrono;

#include "../../../algorithms/hash_functions/HashFunctionsFamily.h"
#include "../data/Data.h"
#include "../data/HashFunctionData.h"
#include "TestCase.h"


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


#endif //SECURE_HASHTESTCASE_H
