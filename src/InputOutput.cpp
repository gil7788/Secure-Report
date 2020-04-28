//
// Created by gil on 16/01/20.
//

#include <iostream>
#include <fstream>
#include "InputOutput.h"


InputOutput::InputOutput(bool output_to_console, const std::string output_file_path, int output_level):
        _output_to_console(output_to_console),
        _output_file_path(output_file_path),
        _output_level(output_level){
    _output_to_file = (not output_file_path.empty());
}


bool InputOutput::output(const std::string& message, int output_level){
    if(not at_output_level(output_level)) {
        return true;
    }

    if(_output_to_console){
        std::cout << message;
    }

    bool created_or_appended_to_file = false;
    if(_output_to_file){
        created_or_appended_to_file = create_or_append_to_file(message, _output_file_path);
    }

    bool output_successful = (not _output_to_file) or created_or_appended_to_file;

    return output_successful;
}


bool InputOutput::create_or_append_to_file(const std::string& message, const std::string& file_path) {
    std::ofstream outfile;
    outfile.open(file_path, std::ios_base::app);
    if(not outfile.is_open()){
        std::cout << "Failed to open file at path: "<< _output_file_path << ".\n Failed to write the following message: " << message << std::endl;
        return false;
    }
    else {
        outfile << message;
        outfile.close();
        return true;
    }
}

bool InputOutput::at_output_level(int message_output_level) {
    return (message_output_level <= _output_level);
}

bool InputOutput::is_file_exist(const std::string file_path) {
    std::ifstream file(file_path.c_str());
    bool file_exists = file.good();
    return file_exists;
}
