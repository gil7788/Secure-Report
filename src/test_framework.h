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
	Matrix<unsigned int> m(out_size, input.size());

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
	std::vector<int> input(input_size);
	for (int i_input = 0; i_input < input_size; ++i_input)
		input[i_input] = 0;
	for (int i_output = 0; i_output < sparsity; ++i_output)
		input[rand() % input_size] = 1;

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
