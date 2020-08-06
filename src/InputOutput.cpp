//
// Created by gil on 16/01/20.
//

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include "InputOutput.h"

namespace fs = std::experimental::filesystem;

InputOutput::InputOutput(bool output_to_console, const std::string& output_file_path, int output_level):
        _output_to_console{output_to_console},
        _output_file_path{output_file_path},
        _output_level{output_level}{
    _output_to_file = {not output_file_path.empty()};
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
        auto absolute_output_file_path = fs::absolute(_output_file_path);
        created_or_appended_to_file = create_or_append_to_file(message, absolute_output_file_path);
    }

    bool output_successful = (not _output_to_file) or created_or_appended_to_file;

    return output_successful;
}

std::string InputOutput::read_file() {
    std::ifstream infile;
    auto absolute_output_file_path = fs::absolute(_output_file_path);
    infile.open(absolute_output_file_path, std::ios_base::app);
    if(not infile.is_open()){
        std::cout << "Failed to open file at path: "<< _output_file_path << ".";
        return "";
    }
    else {
        std::string line;
        std::stringstream file_stream;
        while ( getline (infile,line) ) {
            file_stream << line << '\n';
        }

        infile.close();
        return file_stream.str();
    }
}

bool InputOutput::create_or_append_to_file(const std::string& message, const std::string& file_path) {
    std::ofstream outfile;
    const fs::path path = file_path;
    bool created = fs::create_directories(path.parent_path());
    if(not (fs::is_directory(path.parent_path()) || created)) {
        return false;
    }
    outfile.open(file_path, std::ios_base::ate);
    if(not outfile.is_open()){
        outfile.open(file_path,  std::fstream::in | std::fstream::out | std::fstream::trunc);
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
