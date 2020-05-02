//
// Created by gil on 29/04/20.
//

#include <zp.h>

#include "Database.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

template <typename DataType>
Database<DataType>::Database(int size, int sparsity):
        _size{size},
        _sparsity{sparsity} {}

// Regular database
RegularDatabase::RegularDatabase(int size, int sparsity):
        Database<MyZP>(size, sparsity),
        _data_type(_sparsity){}


FHEDatabase<MyZP> RegularDatabase::build_database() {
    initialize_data_type();
    FHEDatabase<MyZP> fhe_db = FHEDatabase<MyZP>(_size, _sparsity);
    return fhe_db;
}

void RegularDatabase::initialize_data_type() {
    _data_type.initialize_data_type();
}

// Encrypted database
EncryptedDatabase::EncryptedDatabase(int size, int sparsity, EncryptedDataType& data_type):
        Database<HelibNumber>(size, sparsity),
        _data_type(data_type){}

FHEDatabase<HelibNumber> EncryptedDatabase::build_database() {
    initialize_data_type();
    FHEDatabase<HelibNumber> fhe_db = FHEDatabase<HelibNumber>(_size, _sparsity);
    return fhe_db;
}

void EncryptedDatabase::initialize_data_type() {
    _data_type.initialize_data_type();
}
