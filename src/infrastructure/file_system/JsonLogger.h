//
// Created by gil on 1.10.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONLOGDATA_H
#define SECURE_REPORT_HASHFUNCTIONLOGDATA_H

#include "../test/data/HashFunctionData.h"
#include "../JsonParser.h"

using namespace std;

// TODO consider to move to file_system directory

/**
 * Json logger class, output to console and save to file json strings
 */
class JsonLogger {
string _log_name;
vector<unique_ptr<Data>> _data_vector;

public:
    JsonLogger(string& log_name): _log_name{format_log_name(log_name)} {}

    /**
     * Add data element to data vector
     * @param data Data element to add
     */
    void add_data(unique_ptr<Data> data);

    /**
     * Log to file all data elements saved in data vector
     * @return Logged successfully
     */
    bool save_log();

    /**
     * Convert all data elements saved in data vector to json string
     * @return Convert all data elements saved in data vector to json string
     */
    string to_json();

// TODO consider to remove, not used
    string get_log_name();

private:
    fs::path build_log_file_path();

    string format_log_name(string& log_name);
};

#endif //SECURE_REPORT_HASHFUNCTIONLOGDATA_H
