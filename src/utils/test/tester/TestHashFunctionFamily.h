//
// Created by gil on 4.9.2020.
//

#ifndef SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
#define SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H

#include <experimental/filesystem>
#include <malloc.h>

#include "Tester.h"
#include "../../../HashFunctions/HashFunctionsFamily.h"
#include "../../../Config.h"
#include "../../file_system/InputOutput.h"
#include "../JsonLogger.h"
#include "../test_case/HashTestCase.h"

namespace fs = std::experimental::filesystem;

class TestHashFunctionFamily: public Tester {
public:
    explicit TestHashFunctionFamily(string& log_name): Tester::Tester(log_name) {}

    void try_test_and_log(unique_ptr<TestCase>& test_case) override;
};

#endif //SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
