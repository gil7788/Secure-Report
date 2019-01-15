#ifndef SECURE_REPORT_PYTHON_API_H
#define SECURE_REPORT_PYTHON_API_H

#include <iostream>
#include <fstream>
#include <string>
#include <python3.5/Python.h>
#include <vector>

#include <limits.h>
#include <unistd.h>


class python_api {
private:
    wchar_t *getWC(const char *c);

    std::string build_param_list(std::vector<std::string> params_function);

    void load_matrix(double** matrix, std::string file_path, int row_size, int col_size);

public:
    void build_matrix_example();

    int call_python_function(std::string script_file_name, std::string script_function, std::string params_function, std::string python_src_dir);

    int build_matrix(double** matrix, std::string script_file_name, std::string script_function, std::string params_function, std::string python_src_dir,
                     std::string matrix_file_path, int row_size, int col_size);
    // DEBUG
    void print_matrix(double** matrix, int row_size, int col_size);
};


#endif //SECURE_REPORT_PYTHON_API_H



