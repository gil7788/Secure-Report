//
// Created by gil on 1.10.2020.
//

#include "HashFunctionLogData.h"


// TODO rename filename to LogCreator
void LogCreator::add_data(unique_ptr<Data>& data) {
    _data_vector.push_back(move(data));
}

bool LogCreator::save_log() {    
    fs::path file_path = build_log_file_path();
    auto json_file = to_json();

    InputOutput io(constants::OUTPUT_TO_CONSOLE, file_path.u8string(), constants::OUTPUT_LEVEL);

    bool log_saved = io.output(json_file, constants::OUTPUT_LEVELS::LOG);
    return log_saved;
}

fs::path LogCreator::build_log_file_path() {
    auto file_path = fs::path(constants::PROCESSED_FILE_PATH);
    file_path.concat(_log_name);
    return file_path;
}

string LogCreator::to_json() {
    string json = "[ \n";

    for (int i = 0; i < _data_vector.size() - 1; ++i) {
        auto json_element = _data_vector[i].get()->to_json();
        json += json_element + ", \n";
    }
    int last_index = _data_vector.size()-1;
    json += _data_vector[last_index].get()->to_json() + "] \n";
    return json;
}

string LogCreator::get_log_name() {
    return _log_name;
}

string LogCreator::format_log_name(string& log_name) {
    auto log_name_with_underscore = log_name;
    std::replace( log_name_with_underscore.begin(), log_name_with_underscore.end(), ' ', '_' );
    string file_name = log_name_with_underscore + "_log";
    return file_name;
}
