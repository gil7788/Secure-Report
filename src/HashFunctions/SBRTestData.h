//
// Created by gil on 28.11.2020.
//

#ifndef SECURE_SBRTESTDATA_H
#define SECURE_SBRTESTDATA_H


#include "HashFunctionsFamily.h"
#include "TestHashFunctionFamily.h"

class SBRTestData {
private:
    HashFunctionFamily& _hash_function;
    int _database_size;
    int _number_of_matches;
    int _number_of_batches;
    double _average_batch_size;

public:
    SBRTestData(HashFunctionFamily& hash_function, int database_size, int number_of_matches, double average_batch_size);

    void test(TestHashFunctionFamily& tester);

    string to_string();

private:
    int get_number_of_batches(double number_of_batches, int number_of_matches);

    int get_word_length(int number);
};


#endif //SECURE_SBRTESTDATA_H
