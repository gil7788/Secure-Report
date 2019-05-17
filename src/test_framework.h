#ifndef __TEST_FRAMEWORK__
#define __TEST_FRAMEWORK__

#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/types.h>


#include <Eigen/Dense>

using Eigen::MatrixXi;
using Eigen::VectorXi;

#include "matrix.h"
#include "../cpp_sketch/sketch_encoder.h"


/*
 * Description: This framework tests the following flow:
 *
 * 1. Read an array and encrypt it.
 * 2. Read a sketch matrix
 * 4. Create a sketch of the indices where array equals query.  (encrypted)
 * 5. Decrypt sketch.
 * 6. Decode sketch.
 * */
std::vector<int> getMatches(VectorXi x) {
    std::vector<int> indices;
    for (int i = 0; i < x.size(); ++i) {
        if(x[i] == 1) {
            indices.insert(indices.begin(), i);
        }
    }
    return indices;
}


template<class Number>
class InputDatabase {
// Mock class for db, saves plain data but returns encrypted data
private:
    VectorXi _db;

    void copy(const InputDatabase &a) {
        _db(a._db.size());
        _db << a._db;
    }

public:
    InputDatabase(const VectorXi &v): _db(v.size()){
        _db << v;
    }

    InputDatabase(const InputDatabase &a) {
        copy(a);
    }

    InputDatabase &operator=(const InputDatabase &a) {
        copy(a); return *this;
    }

    long size() const {
        return _db.size();
    }

    Number get(int i) const {
        return Number::static_from_int(_db[i]);
    }
};

template<class Number>
inline void encrypted_encode(Number *out, const InputDatabase<Number> &input, sketch_encoder encoder) {
    /* Description:
     * Build sketch vector of matched indices
     * */

    MatrixXi sketch = encoder.sketch;

	for (int i_out = 0; i_out < sketch.rows(); ++i_out)
		out[i_out] = Number::static_from_int(0);

	for (int i_input = 0; i_input < input.size(); ++i_input) {
		Number x = input.get(i_input);

		for (int i_out = 0; i_out < sketch.rows(); ++i_out) {
			if (sketch(i_out, i_input) == 1)
				out[i_out] += x;
		}
	}
}

template<class Number>
bool test_sketch(int input_size, int sparsity) {
    // Init encoder, works only on plain data
    sketch_encoder encoder = sketch_encoder(input_size, sparsity);

    // Initialize X - input vector
	VectorXi input = encoder.generate_binary_vector(input_size, sparsity);
    // Save matched indices for success/failure of test for indication
    std::vector<int> indices = getMatches(input);

    // Create DB
	InputDatabase<Number> input_db(input);

    // Define encrypted result array
    int encoded_size = int(encoder.sketch.rows());
    Number* sketchEnc;
    sketchEnc = new Number[encoded_size];

    // Encode encrypted data
    encrypted_encode(sketchEnc, input_db, encoder);

    // Decrypt sketchEnc (which is encrypted encoding)
    VectorXi encoded(encoded_size);
    for (int i_output = 0; i_output < encoder.sketch.rows(); ++i_output)
		encoded[i_output] = sketchEnc[i_output].to_int();

    // Decode encoded into decoded
    for (int i = 0; i < encoded.size(); ++i) {
        (encoded[i] > 0) ? encoded[i] = 1 : encoded[i] = 0;
    }
    VectorXi decoded = encoder.decode(encoded);

    // Print input and result
    std::cout << "Input indices: " << std::endl;
    for (auto i = indices.begin(); i != indices.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    std::cout << "Decoded: " << decoded << std::endl;

    // Test if result is valid
    std::vector<int> decoded_std(decoded.data(), decoded.data() + decoded.rows() * decoded.cols());
    bool is_successful_decode = std::is_permutation(indices.begin(), indices.end(), decoded_std.begin());

    delete [] sketchEnc;
	return is_successful_decode;
}
#endif
