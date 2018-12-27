#ifndef __TEST_FRAMEWORK__
#define __TEST_FRAMEWORK__

#include <iostream>

#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "matrix.h"

template<class Number>
class InputDatabase {
// Mock class for db
private:
	std::vector<int> _db;

	void copy(const InputDatabase &a) { _db = a._db; }
public:
	InputDatabase(const std::vector<int> &v) : _db(v) {}
	InputDatabase(const InputDatabase &a) { copy(a); }

	InputDatabase &operator=(const InputDatabase &a) { copy(a); return *this; }

	int size() const { return _db.size(); }
	Number get(int i) const { Number::static_from_int(_db[i]); }
};

template<class Number>
inline void sketch(int out_size, Number *out, const InputDatabase<Number> &input) {
    /* Description:
     * Build sketch vector of matched indices
     * */

	Matrix<unsigned int> m(out_size, input.size());

    /*TODO
     * 1. Generate sketch matrix with Saffron.py
     * 2. Write to file result
     * 3. Read from file
     * */

	for (int i_out = 0; i_out < out_size; ++i_out)
		out[i_out] = Number::static_from_int(0);

	for (int i_input = 0; i_input < input.size(); ++i_input) {
		Number x = input.get(i_input);

		for (int i_out = 0; i_out < out_size; ++i_out) {
            printf("i_out: %d \n", i_out);
			if (m(i_out, i_input) == 1)
				out[i_out] += x;
		}
	}
}

template<class Number>
bool test_sketch(int input_size, int sparsity) {
    // Initialize X - input vector
	std::vector<int> input(input_size);
	for (int i_input = 0; i_input < input_size; ++i_input)
		input[i_input] = 0;
	for (int i_output = 0; i_output < sparsity; ++i_output)
		input[rand() % input_size] = 1;

    // Initialize db
	InputDatabase<Number> input_db(input);
	Number sketchEnc[sparsity];
	sketch(sparsity, sketchEnc, input_db);

	int sketch[sparsity];

	for (int i_output = 0; i_output < sparsity; ++i_output)
		sketch[i_output] = sketchEnc[i_output].to_int();

	std::vector<int> decoded(input_size);

	// TODO:
	// decode sketch into decoded

	for (int i_input = 0; i_input < input_size; ++i_input)
		if (decoded[i_input] != input[i_input])
			return false;

	return true;
}


#endif
