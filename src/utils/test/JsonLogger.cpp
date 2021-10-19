//
// Created by gil on 1.10.2020.
//

#include "JsonLogger.h"


void JsonLogger::add_data(unique_ptr<Data> data) {
    _data_vector.push_back(move(data));
}

bool JsonLogger::save_log() {
    fs::path file_path = build_log_file_path();
    auto json_file = to_json();

    InputOutput io(constants::OUTPUT_TO_CONSOLE, file_path.u8string(), constants::OUTPUT_LEVEL);

    bool log_saved = io.output(json_file, constants::OUTPUT_LEVELS::LOG);
    return log_saved;
}

fs::path JsonLogger::build_log_file_path() {
    auto file_path = fs::path(constants::PROCESSED_FILE_PATH);
    file_path.concat(_log_name);
    return file_path;
}

string JsonLogger::to_json() {
    vector<string> json_elements;
    for (int i = 0; i < _data_vector.size(); ++i) {
        auto json_element = _data_vector[i].get()->to_json();
        json_elements.push_back(json_element);
    }
    auto json_array = JsonParser::vector_to_json_string(json_elements);
    return json_array;
}

string JsonLogger::get_log_name() {
    return _log_name;
}

string JsonLogger::format_log_name(string& log_name) {
    auto log_name_with_underscore = log_name;
    std::replace( log_name_with_underscore.begin(), log_name_with_underscore.end(), ' ', '_' );
    string file_name = log_name_with_underscore + "_log";
    return file_name;
}
