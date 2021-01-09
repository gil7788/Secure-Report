//
// Created by gil on 1.10.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONLOGDATA_H
#define SECURE_REPORT_HASHFUNCTIONLOGDATA_H

#include "HashFunctionData.h"

using namespace std;

// TODO rename filename to LogCreator
class LogCreator {
string _log_name;
vector<unique_ptr<Data>> _data_vector;

public:
    LogCreator(string& log_name): _log_name{format_log_name(log_name)} {}

    void add_data(unique_ptr<Data>& data);

    bool save_log();

    fs::path build_log_file_path();

    string to_json();

protected:

    string get_log_name();

    string format_log_name(string& log_name);
};

#endif //SECURE_REPORT_HASHFUNCTIONLOGDATA_H
