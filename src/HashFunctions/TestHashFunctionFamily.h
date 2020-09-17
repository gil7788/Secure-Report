//
// Created by gil on 4.9.2020.
//

#ifndef SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
#define SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H

#include <experimental/filesystem>
#include <chrono>

#include "HashFunctionsFamily.h"
#include "../Config.h"
#include "../InputOutput.h"

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
    vector<int> get_batches_size(vector<int>& split, int number_of_matches);

    double vector_mean(vector<int>& v);

    double diviation(vector<int>& v);

public:
    string log_result(HashFunctionFamily& hash_function, vector<int> &evaluated_domain,
            Duration& build_time_duration, Duration& evaluation_time_duration);

    string test_hash_function(HashFunctionFamily & h, vector<int>& indices_indicator);

    vector<int> test_subset(HashFunctionFamily& hash_function, vector<int>& subset);

    string test_single_instance(HashFunctionFamily& hash_function, int domain_word_length,
                                int range_word_length, int number_of_matches);

    std::vector<int> sample_indices(int number_of_indices);
};

#endif //SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
