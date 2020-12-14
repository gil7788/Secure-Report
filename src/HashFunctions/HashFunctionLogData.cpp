//
// Created by gil on 1.10.2020.
//

#include "HashFunctionLogData.h"

void HashFunctionLogData::add_data(HashFunctionData& data) {
    _hash_function_data_vector.push_back(data);
}

bool HashFunctionLogData::save_log() {
    auto hash_name = _hash_function_data_vector[0]._hash_name;
    fs::path file_path = build_log_file_path(hash_name);
    auto json_file = to_json();

    InputOutput io(constants::OUTPUT_TO_CONSOLE, file_path.u8string(), constants::OUTPUT_LEVEL);

    bool log_saved = io.output(json_file, constants::OUTPUT_LEVELS::LOG);
    return log_saved;
}

fs::path HashFunctionLogData::build_log_file_path(string &hash_function_name) {
    auto hash_function_name_with_underscore = hash_function_name;
    std::replace( hash_function_name_with_underscore.begin(), hash_function_name_with_underscore.end(), ' ', '_' );
    string file_name = hash_function_name_with_underscore + "_log";
    auto file_path = fs::path(constants::PROCESSED_FILE_PATH);
    file_path.concat(file_name);
    return file_path;
}


string HashFunctionLogData::to_json() {
    string json = "[ \n";

    for (int i = 0; i < _hash_function_data_vector.size() - 1; ++i) {
        json += _hash_function_data_vector[i].to_json() + ", \n";
    }
    int last_index = _hash_function_data_vector.size()-1;
    json += _hash_function_data_vector[last_index].to_json() + "] \n";
    return json;
}
