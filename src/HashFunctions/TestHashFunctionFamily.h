//
// Created by gil on 4.9.2020.
//

#ifndef SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
#define SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H

#include <experimental/filesystem>
#include <chrono>
#include <malloc.h>

#include "HashFunctionsFamily.h"
#include "../Config.h"
#include "../InputOutput.h"
#include "HashFunctionLogData.h"

namespace fs = std::experimental::filesystem;
using namespace std::chrono;

class Duration {
private:
    enum class TIME_UNIT {NS, MS, S, M, H};
    double _duration;
    TIME_UNIT _time_unit;

private:
    void normalize_duration(std::chrono::duration<double> duration);

public:
    explicit Duration(std::chrono::duration<double> duration);

    string to_string();

    friend ostream& operator<<(ostream& os, const Duration& duration);
};

class TestHashFunctionFamily {
private:
    HashFunctionLogData _data;
public:
    bool log_result();

    void test_hash_function(HashFunctionFamily & h, vector<int>& indices_indicator, int number_of_matches);

    vector<int> test_subset(HashFunctionFamily& hash_function, vector<int>& subset);

    void test_single_instance(HashFunctionFamily& hash_function, int domain_word_length,
                                int range_word_length, int number_of_matches);

    void try_test_hash_function_and_log(HashFunctionFamily& hash_function, int domain_word_length,
                                        int number_of_batches_word_length, int number_of_matches_word_length);

    std::vector<int> sample_indices(int number_of_indices, int range_upper_bound);
};

#endif //SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
