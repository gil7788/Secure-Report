//
// Created by gil on 4.9.2020.
//

#include "TestHashFunctionFamily.h"
#include "../data/HashFunctionData.h"


void TestHashFunctionFamily::try_test_and_log(unique_ptr<TestCase>& test_case) {
    try {
        unique_ptr<Data> test_result = move(test_case->test());
        Tester::add_test_run_to_log(test_result);
    }
    catch (const std::exception& e) {
        string error_string = "Failed to test hash function " + test_case->to_string() + "\n";
        cerr << error_string;
    }

    log_result();
}
