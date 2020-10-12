//
// Created by gil on 16/01/20.
//

#ifndef SECURE_REPORT_INPUTOUTPUT_H
#define SECURE_REPORT_INPUTOUTPUT_H

#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class InputOutput {
    const bool _output_to_console;
    const fs::path _output_file_path;
    const int _output_level;
    bool _output_to_file;

public:
    InputOutput(bool output_to_console, const std::string& output_file_path, int output_level);
    InputOutput(bool output_to_console, const fs::path& output_file_path, int output_level);
    bool output(const std::string& message, int output_level);
    bool create_or_append_to_file(const std::string& message);
    std::string read_file();
    static bool is_file_exist(const std::string file_path);
private:
    bool at_output_level(int output_level);
};


#endif //SECURE_REPORT_INPUTOUTPUT_H
