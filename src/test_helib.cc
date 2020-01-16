#include <iostream>

#include <helib_number.h>
#include <eq.h>

#include "FHEDatabase.h"

int main(int argc, char**argv) {
 	HelibKeys keys;

    // Default args
	long r = 1;
	int L = 7;
	int size = 128;
	int p = Primes::find_prime_bigger_than(size);
	int sparsity = 3;

	long R = 1;
	long d = 1;
	long c = 2;
	long k = 80;
	long s = 0;
	long chosen_m = 0;
	Vec<long> gens;
	Vec<long> ords;

	std::cout << "Using p =  " << p << std::endl;
	std::cout << "Using L =  " << L << std::endl;

	keys.initKeys(s, R, p, r, d, c, k, 64, L, chosen_m, gens, ords);
	HelibNumber::set_global_keys(&keys);

    // Test sketch
    FHEDatabase<HelibNumber> fhe_db = FHEDatabase<HelibNumber>(size, sparsity);
    bool is_successful = fhe_db.test_sketch();
    std::cout << "is_successful: " << is_successful << std::endl;
}
