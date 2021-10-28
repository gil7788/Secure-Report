//
// Created by gil on 9.1.2021.
//

#include "Tester.h"

void Tester::initialize(vector<unique_ptr<TestCase>>& test_cases) {
    for (auto& test: test_cases) {
        _test_cases.push_back(move(test));
    }
}

void Tester::test_and_log_all() {
    for (auto &test : _test_cases) {
        try_test_and_log(test);
    }
}

void Tester::test_element(unique_ptr<TestCase>& test) {
    test.get()->test();
}

bool Tester::log_result() {
    bool log_saved = _log_data.save_log();
    return log_saved;
}

void Tester::add_test_run_to_log(unique_ptr<Data> test_result) {
    _log_data.add_data(move(test_result));
}