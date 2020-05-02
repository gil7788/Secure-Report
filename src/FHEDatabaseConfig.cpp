//
// Created by gil on 28/04/20.
//

#include <helib_number.h>
#include <zp.h>
#include <primes.h>
#include "FHEDatabaseConfig.h"
#include "FHEDatabase.h"


#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

// Data type constructors
PlainDataType::PlainDataType(int database_matches_sparsity):
        _database_matches_sparsity{database_matches_sparsity} {}

void PlainDataType::initialize_data_type() {
    int prime = Primes::find_prime_bigger_than(_database_matches_sparsity);
    MyZP::set_global_p(prime);
}

DATA_TYPES PlainDataType::get_data_type() {
    return DATA_TYPES::PLAIN;
}

EncryptedDataType::EncryptedDataType(int database_size, long s, long R, long r,
                  long d, long c, long k, int L, long chosen_m, Vec<long>& gens, Vec<long>& ords):
        _database_size(database_size),
        _s(s),
        _R(R),
        _r(r),
        _d(d),
        _c(c),
        _k(k),
        _L(L),
        _chosen_m(chosen_m),
        _gens(gens),
        _ords(ords){}

DATA_TYPES EncryptedDataType::get_data_type() {
    return DATA_TYPES::ENCRYPTED;
}

void EncryptedDataType::initialize_data_type() {
    int prime = Primes::find_prime_bigger_than(_database_size);

    _keys.initKeys(_s, _R, prime, _r, _d, _c, _k, 64, _L, _chosen_m, _gens, _ords);
    HelibNumber::set_global_keys(&_keys);
}
