#include <iostream>
#include <string.h>

#include <zp.h>
#include <eq.h>

#include "test_framework.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

int main(int argc, char** argv) {
	int size = 256;
	int sparsity = 6;

	for (int argc_i = 0; argc_i < argc; ++argc_i) {
		if (memcmp(argv[argc_i], "--s=", 4) == 0)
			sparsity = atoi(argv[argc_i] + 4);
		if (memcmp(argv[argc_i], "--size=", 7) == 0)
			size = atoi(argv[argc_i] + 7);

		if (strcmp(argv[argc_i], "--help") == 0) {
			std::cout << "   --s= sparsity" << std::endl;
			std::cout << "   --size= size of input" << std::endl;
		}
	}

	int SIMD_factor = 1;

	long p = Primes::find_prime_bigger_than(sparsity);
	MyZP::set_global_p(p);

	std::cout << "Going over an array with " << size << " entries " << "which is " << sparsity << " sparse" << std::endl;
	std::cout << "using p =  " << p << std::endl;
	
	FHEDatabase<MyZP> fhe_db = FHEDatabase<MyZP>(size, sparsity);
	bool is_successful = fhe_db.test_sketch();
	std::cout << "is_successful: " << is_successful << std::endl;
}
