//
// Created by gil on 1.10.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONLOGDATA_H
#define SECURE_REPORT_HASHFUNCTIONLOGDATA_H

#include "HashFunctionData.h"

using namespace std;

class HashFunctionLogData {
public:
    vector<HashFunctionData> _hash_function_data_vector;

    void add_data(HashFunctionData& data);

    bool save_log();

    fs::path build_log_file_path(string& hash_function_name);

    string to_json();
};


#endif //SECURE_REPORT_HASHFUNCTIONLOGDATA_H
