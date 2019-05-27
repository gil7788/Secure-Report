#include <iostream>
#include <string.h>

#include <helib_number.h>
#include <eq.h>

#include "test_framework.h"

int main(int argc, char**argv) {
    /*TODO
     * 1. Understand parameters
     * 2. Consider whether to export parameters to FHEDatabase class or
     * keep FHEDatabase as a generic Database and create Database for each type of FHE library, i.e. Helib, ZP(plain text HElib interface), etc
     * */

	HelibKeys keys;

    // Default args
	int p = 0;
	long r = 1;
	int L = 0;
	int size = 256;
	int sparsity = 3;

    // Parse args
	for (int argc_i = 0; argc_i < argc; ++argc_i) {
		if (memcmp(argv[argc_i], "--r=", 4) == 0)
			r = atoi(argv[argc_i] + 4);
		if (memcmp(argv[argc_i], "--p=", 4) == 0)
			p = atoi(argv[argc_i] + 4);
		if (memcmp(argv[argc_i], "--s=", 4) == 0)
			sparsity = atoi(argv[argc_i] + 4);
		if (memcmp(argv[argc_i], "--L=", 4) == 0)
			L = atoi(argv[argc_i] + 4);
		if (memcmp(argv[argc_i], "--size=", 7) == 0)
			size = atoi(argv[argc_i] + 7);

		if (strcmp(argv[argc_i], "--help") == 0) {
			std::cout << "   --L= levels" << std::endl;
			std::cout << "   --p= ring size" << std::endl;
			std::cout << "   --s= sparsity" << std::endl;
			std::cout << "   --r=" << std::endl;
			std::cout << "   --size= size of input" << std::endl;
		}
	}

	if (L == 0) {
		int LL = 1;
		while ((1 << LL) < p)
			++LL;

		L = 1;
		while ((1 << L) < LL)
			++L;
	}

	long R = 1;
	long d = 1;
	long c = 2;
	long k = 80;
	long s = 0;
	long chosen_m = 0;
	Vec<long> gens;
	Vec<long> ords;

	if (p == 0)
		p = Primes::find_prime_bigger_than(size);

	std::cout << "using p =  " << p << std::endl;
	std::cout << "using L =  " << L << std::endl;

	keys.initKeys(s, R, p, r, d, c, k, 64, L, chosen_m, gens, ords);
	HelibNumber::set_global_keys(&keys);

    // Test sketch
    FHEDatabase<HelibNumber> fhe_db = FHEDatabase<HelibNumber>(size, sparsity);
    bool is_successful = fhe_db.test_sketch();
    std::cout << "is_successful: " << is_successful << std::endl;
}
