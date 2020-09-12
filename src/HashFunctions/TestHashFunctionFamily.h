//
// Created by gil on 4.9.2020.
//

#ifndef SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
#define SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H

#include "HashFunctionsFamily.h"

class TestHashFunctionFamily {
private:
    vector<int> get_batches_size(vector<int>& split, int number_of_matches);

    double vector_mean(vector<int>& v);

    double diviation(vector<int>& v);

public:
    void log_result(HashFunctionFamily& hash_function, vector<int> &evaluated_domain);

    void test_hash_function(HashFunctionFamily & h, vector<int>& indices_indicator, int batch_size_upper_bound);

    vector<int> test_subset(HashFunctionFamily& hash_function, vector<int>& subset);
};


#endif //SECURE_REPORT_TESTHASHFUNCTIONFAMILY_H
