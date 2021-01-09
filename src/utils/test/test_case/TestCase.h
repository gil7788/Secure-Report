//
// Created by gil on 9.1.2021.
//

#ifndef SECURE_TESTCASE_H
#define SECURE_TESTCASE_H

#include <iostream>
#include <memory>

#include "../data/Data.h"

using namespace std;

class TestCase {
public:
    virtual ~TestCase() {};

    virtual string to_string() = 0;

    virtual unique_ptr<Data> test() = 0;
};


#endif //SECURE_TESTCASE_H
