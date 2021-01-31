#ifndef PROTOCOL_TESTER_H
#define PROTOCOL_TESTER_H

#include "Tester.h"
#include "../../../Config.h"

namespace fs = std::experimental::filesystem;

class ProtocolTester: public Tester {
public:
    explicit ProtocolTester(string& log_name): Tester::Tester(log_name) {}

    void try_test_and_log(unique_ptr<TestCase>& test_case) override;
};

#endif
