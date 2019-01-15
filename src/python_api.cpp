#include "python_api.h"

/*
 * Description:
 * Example of the following flow:
 * 1. Create matrix with Python
 * 2. Write matrix to file with Python
 * 3. Read matrix with c++
 * */

std::string python_api::array_to_python_array(int arr[], int arr_size) {
    std::string result = "[ ";
    for(int i = 0; i < arr_size; ++i) {
        if(i == arr_size - 1) {
            result += std::to_string(arr[i]);
        } else {
            result += std::to_string(arr[i]) + ", ";
        }
    }
    result += " ]";
    return result;
}

wchar_t* python_api::getWC(const char *c) {
    /*
     * Description: converts char types
     */
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

std::string python_api::build_param_list(std::vector<std::string> params_function) {
    /*
     * Description: build valid params string from vector<std::string> params_function
     */
    std::string result;
    for(int i = 0; i < params_function.size(); ++i) {
        if(i == params_function.size() - 1) {

            result += params_function[i];
        }
        else {
            result += params_function[i] + ", ";
        }

    }
    return result;
}

void python_api::load_matrix(double** matrix, std::string file_path, int row_size, int col_size) {
    /*
     * Description: loads matrix from file_path to matrix, assumes matrix size is known
     */
    int x, y;
    std::ifstream in(file_path);

    if (!in) {
        std::cout << "Cannot open file.\n";
        return;
    }

    for (y = 0; y < col_size; y++) {
        for (x = 0; x < row_size; x++) {
            in >> matrix[x][y];
        }
    }

    in.close();
}

void python_api::build_matrix_example() {
    /*
     * Description: Example of desired flow
     * 1. Create matrix with Python
     * 2. Write matrix to file with Python
     * 3. Read matrix with c++
     */
    const int MATRIX_ROW = 3;
    const int MATRIX_COL = 3;
    std::string proj_dir = "/home/gil/University/semester19a/crypto_lab/work/secure_report/";
    std::string python_src_dir = proj_dir + "python_sketch/";
    std::string matrix_file = "matrix.txt";
    std::string matrix_file_path = proj_dir + "processed/" + matrix_file;
    std::string script_file_name = "write_matrix_to_file";
    std::vector<std::string> params_function(1, matrix_file_path);
    std::string script_function = "file_test";

    // Init new matrix
    auto** matrix = new double*[MATRIX_ROW];
    for(int i = 0; i < MATRIX_ROW; ++i)
        matrix[i] = new double[MATRIX_COL];


    std::string params = build_param_list(params_function);
    build_matrix(matrix, script_file_name, script_function, params, python_src_dir, matrix_file_path, MATRIX_ROW, MATRIX_COL);
    print_matrix(matrix, MATRIX_ROW, MATRIX_COL);
}

int python_api::call_python_function(std::string script_file_name, std::string script_function, std::string params_function, std::string python_src_dir) {
    /*
     * Description: Init Python venv interpreter and call Python function that calculates and saves matrix to file
     * Create file relative to script call path
     */
    std::string venv_activation_path = python_src_dir + "venv/bin/python";
    // Set Interpreter
    Py_SetProgramName(getWC(venv_activation_path.c_str()));
    Py_Initialize();

    // Include modules/files that are about to run
    PyRun_SimpleString("import sys");
    std::string cmd_include_module = ("sys.path.insert(0, \"" + python_src_dir + "\")");
    PyRun_SimpleString(cmd_include_module.c_str());
    PyRun_SimpleString(("import " + script_file_name).c_str());

    // Run script
    std::string cmd_run_script_function = script_file_name + "." + script_function + "(" + params_function + ")";
    PyRun_SimpleString(cmd_run_script_function.c_str());

    return 0;
}

int python_api::build_matrix(double** matrix, std::string script_file_name, std::string script_function, std::string params_function, std::string python_src_dir,
                 std::string matrix_file_path, int row_size, int col_size) {
    /*
     * Description:
     * 1. Calculate matrix and save to file
     * 2. Load matrix from file
     * 3. Print matrix
     * Note: Assume "processed" directory exist
     */

    // Calculate matrix and save to file
    int python_call_status = call_python_function(script_file_name, script_function, params_function, python_src_dir);
    if(python_call_status != 0) {
        std::cout << "Failed to call python script" << std::endl;
        return python_call_status;
    }

    // Load matrix from file
    load_matrix(matrix, matrix_file_path, row_size, col_size);
    return 0;
}

void python_api::print_matrix(double** matrix, int row_size, int col_size) {
    /*
     * Description: Print matrix
     */
    for(int i = 0; i < row_size; ++i) {
        for(int j = 0; j < col_size; ++j) {
            std::cout << matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}
/*
int main() {
    python_api py;
    py.build_matrix_example();

}*/