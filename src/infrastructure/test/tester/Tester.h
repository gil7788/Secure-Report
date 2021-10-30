//
// Created by gil on 9.1.2021.
//

#ifndef SECURE_TESTER_H
#define SECURE_TESTER_H

#include "../../file_system/JsonLogger.h"
#include "../test_case/HashTestCase.h"

class Tester {
private:
    JsonLogger _log_data;
    vector<unique_ptr<TestCase>> _test_cases;

public:
    explicit Tester(string& log_name): _log_data(log_name) {}

    virtual ~Tester() {};

    void initialize(vector<unique_ptr<TestCase>>& test_cases);

    void test_and_log_all();

protected:
    bool log_result();

    void add_test_run_to_log(unique_ptr<Data> test_result);

    virtual void try_test_and_log(unique_ptr<TestCase>& test_case) = 0;
};

#endif //SECURE_TESTER_H
