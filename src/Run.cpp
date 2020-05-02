//
// Created by gil on 28/04/20.
//

#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"
#include "Database.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

template <typename T>
bool test_database(Database<T>& database) {
    auto fhe_db = database.build_database();
    bool is_successful = fhe_db.test_sketch();
    return is_successful;
}

void test_plain_database(RegularDatabase& database) {
    bool is_successful = test_database(database);
    std::cout << "Plain Success: " << is_successful << std::endl;
}

void test_encrypted_database(EncryptedDatabase& database) {
    bool is_successful = test_database(database);
    std::cout << "Encrypted Success: " << is_successful << std::endl;
}

int main(int argc, char** argv) {
    int size = 128;
    int sparsity = 3;

    long r = 1;
    int L = 7;
    int p = Primes::find_prime_bigger_than(size);

    long R = 1;
    long d = 1;
    long c = 2;
    long k = 80;
    long s = 0;
    long chosen_m = 0;
    Vec<long> gens;
    Vec<long> ords;

    RegularDatabase plain_database(size, sparsity);
    EncryptedDataType encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords);
    EncryptedDatabase encrypted_database(size, sparsity, encrypted_data_type);

    test_plain_database(plain_database);
//    test_encrypted_database(encrypted_database);
}
