#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEDatabase.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

int main(int argc, char** argv) {
	int size = 128;
	int sparsity = 3;

	int SIMD_factor = 1;

	long p = Primes::find_prime_bigger_than(sparsity);
	MyZP::set_global_p(p);

	std::cout << "Input array is " << sparsity << " sparse and of size: " << size << std::endl;
	std::cout << "Using p =  " << p << std::endl;

	FHEDatabase<MyZP> fhe_db = FHEDatabase<MyZP>(size, sparsity);
	bool is_successful = fhe_db.test_sketch();
	std::cout << "Success: " << is_successful << std::endl;
}
