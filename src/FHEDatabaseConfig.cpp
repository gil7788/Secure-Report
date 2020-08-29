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

namespace fs = std::experimental::filesystem;

GenericPlainDataType::GenericPlainDataType(int r): _r(r) {}

void GenericPlainDataType::initialize() {
    int modulo = 2;
    GenericZP::set_global_p(modulo, _r);
}

DATA_TYPES GenericPlainDataType::get_data_type() {
    return DATA_TYPES::PLAIN;
}

void GenericPlainDataType::write_key_to_file(const std::string& key_file_path) {}

void GenericPlainDataType::read_key_from_file(const std::string& key_file_path) {}

// EncryptedDataTypeFromParameters
EncryptedDataTypeFromParameters::EncryptedDataTypeFromParameters(int database_size, long s, long R, long r,
                  long d, long c, long k, int L, long chosen_m, Vec<long>& gens, Vec<long>& ords,
                  const std::string& key_file_path):
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
        _ords(ords),
        _key_file_path{key_file_path}{}

DATA_TYPES EncryptedDataTypeFromParameters::get_data_type() {
    return DATA_TYPES::ENCRYPTED;
}

void EncryptedDataTypeFromParameters::initialize() {
    if(fs::is_regular_file(_key_file_path)) {
        read_key_from_file(_key_file_path);
        SimplifiedHelibNumber::set_global_keys(&_keys);
    }
    else {
        int modulo = 2;

        _keys.initKeys(_s, _R, modulo, _r, _d, _c, _k, 64, _L, _chosen_m, _gens, _ords);
        SimplifiedHelibNumber::set_global_keys(&_keys);
        write_key_to_file(_key_file_path);
    }
}

void EncryptedDataTypeFromParameters::write_key_to_file(const std::string& key_file_path) {
    std::ofstream key_file_path_stream {key_file_path};
    _keys.write_to_file(key_file_path_stream);
}

void EncryptedDataTypeFromParameters::read_key_from_file(const std::string &key_file_path) {
    std::ifstream key_file_path_stream {key_file_path};
    _keys.read_from_file(key_file_path_stream);
}
