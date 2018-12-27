#ifndef __TEST_FRAMEWORK__
#define __TEST_FRAMEWORK__

#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "matrix.h"


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
inline void sketch(int out_size, Number *out, const InputDatabase<Number> &input) {
    /* Description:
     * Build sketch vector of matched indices
     * */

	Matrix<unsigned int> m(out_size, input.size());

    // TODO: 2. Read a sketch matrix
    /*
     * a. Generate sketch matrix with Saffron.py
     * b. Write to file result
     * c. Read from file
     * */

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

    // 2. Read a sketch matrix, 4. Decrypt sketch.
    // Decrypted result
	int sketch[sparsity];
    // Decrypt sketchEnc (which is encrypted result array)
	for (int i_output = 0; i_output < sparsity; ++i_output)
		sketch[i_output] = sketchEnc[i_output].to_int();

    // TODO: 5. Decode sketch.
    // decode sketch into decoded
	std::vector<int> decoded(input_size);

    // Test if result is valid
	for (int i_input = 0; i_input < input_size; ++i_input)
		if (decoded[i_input] != input[i_input])
			return false;

	return true;
}


#endif
