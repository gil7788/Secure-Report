#ifndef __TEST_FRAMEWORK__
#define __TEST_FRAMEWORK__

#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "matrix.h"
#include "python_api.h"


/*
 * Description: This framework tests the following flow:
 *
 * 1. Read an array and encrypt it.
 * 2. Read a sketch matrix
 * 4. Create a sketch of the indices where array equals query.  (encrypted)
 * 5. Decrypt sketch.
 * 6. Decode sketch.
 * */

template<class Number>
class InputDatabase {
// Mock class for db, saves plain data but returns encrypted data
private:
	std::vector<int> _db;

	void copy(const InputDatabase &a) { _db = a._db; }
public:
	InputDatabase(const std::vector<int> &v) : _db(v) {}
	InputDatabase(const InputDatabase &a) { copy(a); }

	InputDatabase &operator=(const InputDatabase &a) { copy(a); return *this; }

	int size() const { return _db.size(); }
    Number get(int i) const { return Number::static_from_int(_db[i]); }
};

template<class Number>
inline Matrix<unsigned int> build_sketch_matrix(int out_size, Number *out, const InputDatabase<Number> &input) {
    std::cout << "Started building matrix" << std::endl;
    // TODO: 2. Read a sketch matrix
    /*
     * a. Generate sketch matrix with Saffron.py
     * b. Write to file result
     * c. Read from file
     * */

    /*
     * Description: Example of desired flow
     * 1. Create matrix with Python
     * 2. Write matrix to file with Python
     * 3. Read matrix with c++
     */

    const int MATRIX_ROW = input.size();
    const int MATRIX_COL = out_size;
    std::string proj_dir = "/home/gil/University/semester19a/crypto_lab/work/secure_report/";
    std::string python_src_dir = proj_dir + "python_sketch/";
    std::string matrix_file = "matrix.txt";
    std::string matrix_file_path = proj_dir + "processed/" + matrix_file;
    std::string matrix_file_path_param = "\"" + proj_dir + "processed/" + matrix_file + "\"";

    std::string script_file_name = "saffron";
	std::string params_array[] = {std::to_string(MATRIX_ROW), std::to_string(MATRIX_COL), matrix_file_path_param};
    std::vector<std::string> params_function(params_array, params_array + sizeof(params_array) / sizeof(std::string));
    std::string script_function = "save_sketch_to_file";

    python_api py;

    // Init new matrix
    auto** matrix = new double*[MATRIX_ROW];
    for(int i = 0; i < MATRIX_ROW; ++i)
        matrix[i] = new double[MATRIX_COL];

    std::string params = py.build_param_list(params_function);
    py.build_matrix(matrix, script_file_name, script_function, params, python_src_dir, matrix_file_path, MATRIX_ROW, MATRIX_COL);
    py.print_matrix(matrix, MATRIX_ROW, MATRIX_COL);

    Matrix<unsigned int> result = Matrix<unsigned int>(matrix,MATRIX_ROW, MATRIX_COL);
    for(int i = 0; i < MATRIX_ROW; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return result;

}

template<class Number>
inline std::vector<int> decode_sketch_matrix(int out_size, const InputDatabase<Number> &input, int sketch_vector[]) {
    std::cout << "Started decoding" << std::endl;
    // TODO: 5. Decode sketch.
    // decode sketch into decoded
    python_api py;
    const int MATRIX_ROW = input.size();
    const int MATRIX_COL = out_size;
    std::string proj_dir = "/home/gil/University/semester19a/crypto_lab/work/secure_report/";
    std::string python_src_dir = proj_dir + "python_sketch/";
    std::string matrix_file = "decoded_vector.txt";
    std::string matrix_file_path = proj_dir + "processed/" + matrix_file;
    std::string matrix_file_path_param = "\"" + proj_dir + "processed/" + matrix_file + "\"";

    std::string script_file_name = "saffron";
    std::string params_array[] = {std::to_string(MATRIX_ROW), std::to_string(MATRIX_COL),
                                  py.array_to_python_array(sketch_vector, MATRIX_COL), matrix_file_path_param};
    std::vector<std::string> params_function(params_array, params_array + sizeof(params_array) / sizeof(std::string));
    std::string script_function = "decode_save_to_file";

    auto** vector = new double*[0];
    vector[0] = new double[MATRIX_ROW];

    std::string params = py.build_param_list(params_function);
    py.build_matrix(vector, script_file_name, script_function, params, python_src_dir, matrix_file_path, MATRIX_ROW, MATRIX_COL);
    py.print_matrix(vector, MATRIX_ROW, MATRIX_COL);

    std::vector<int> result_vector;
    for(int i = 0;i < MATRIX_ROW; ++i) {
        result_vector.push_back((int)vector[0][i]);
    }
    return result_vector;
}

template<class Number>
inline void sketch(int out_size, Number *out, const InputDatabase<Number> &input) {
    /* Description:
     * Build sketch vector of matched indices
     * */

	Matrix<unsigned int> m(out_size, input.size());

    // TODO: 2. Read a sketch matrix
    m = build_sketch_matrix(out_size, out, input);

	for (int i_out = 0; i_out < out_size; ++i_out)
		out[i_out] = Number::static_from_int(0);

	for (int i_input = 0; i_input < input.size(); ++i_input) {
		Number x = input.get(i_input);

		for (int i_out = 0; i_out < out_size; ++i_out) {
			if (m(i_out, i_input) == 1)
				out[i_out] += x;
		}
	}
}

template<class Number>
bool test_sketch(int input_size, int sparsity) {
    /* Description: Implement 1-6:
     * 1. Read an array and encrypt it.
     * 2. Read a sketch matrix
     * 3. Create a sketch of the indices where array equals query.  (encrypted)
     * 4. Decrypt sketch.
     * 5. Decode sketch.
     * */

    // Initialize X - input vector
	std::vector<int> input(input_size);
	for (int i_input = 0; i_input < input_size; ++i_input)
		input[i_input] = 0;
	for (int i_output = 0; i_output < sparsity; ++i_output)
		input[rand() % input_size] = 1;

    // 1. Read an array and encrypt it.
	InputDatabase<Number> input_db(input);
    // Define encrypted result array
	Number sketchEnc[sparsity];
    // 3. Create a sketch of the indices where array equals query.  (encrypted)
	sketch(sparsity, sketchEnc, input_db);

	// Build sketch matrix for decoding
	Matrix<unsigned int> m(sparsity, input_db.size());
	m = build_sketch_matrix(sparsity, sketchEnc, input_db);

    // 2. Read a sketch matrix, 4. Decrypt sketch.
    // Decrypted result
	int sketch[sparsity];
    // Decrypt sketchEnc (which is encrypted result array)
	for (int i_output = 0; i_output < sparsity; ++i_output)
		sketch[i_output] = sketchEnc[i_output].to_int();

    // TODO: 5. Decode sketch.
    // decode sketch into decoded
	std::vector<int> decoded(input_size);

    decoded = decode_sketch_matrix(sparsity, input_db, sketch);
    // Test if result is valid
	for (int i_input = 0; i_input < input_size; ++i_input)
		if (decoded[i_input] != input[i_input])
			return false;

	return true;
}


#endif
