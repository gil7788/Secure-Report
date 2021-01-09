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
#include "SBRTestData.h"

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


class Tester {
private:
    LogCreator _log_data;
    vector<unique_ptr<TestCase>> _test_cases;

public:
    explicit Tester(string& log_name): _log_data(log_name) {}

    virtual ~Tester() {};

    void initialize(vector<unique_ptr<TestCase>>& test_cases);

    void test_and_log_all();

protected:
    bool log_result();

    void add_test_run_to_log(unique_ptr<Data>& test_result);

    virtual void try_test_and_log(unique_ptr<TestCase>& test_case) = 0;

    void test_element(unique_ptr<TestCase>& test);
};


class TestHashFunctionFamily: public Tester {
public:
    explicit TestHashFunctionFamily(string& log_name): Tester::Tester(log_name) {}

    // void test_hash_function(HashFunctionFamily & h, vector<int>& indices_indicator, int number_of_matches);

    // vector<int> test_subset(HashFunctionFamily& hash_function, vector<int>& subset);

    // void test_single_instance(HashFunctionFamily& hash_function, int domain_word_length,
    //                             int range_word_length, int number_of_matches);

    void try_test_and_log(unique_ptr<TestCase>& test_case) override;

    // void try_test_hash_function_and_log(HashFunctionFamily& hash_function, int domain_word_length,
    //                                     int number_of_batches_word_length, int number_of_matches_word_length);

    // std::vector<int> sample_indices(int number_of_indices, int range_upper_bound);
};

#endif //SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
