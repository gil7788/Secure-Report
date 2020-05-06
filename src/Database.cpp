//
// Created by gil on 29/04/20.
//

#include <zp.h>

#include "Database.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

// Regular database
RegularDatabase::RegularDatabase(int size, int sparsity, PlainDataType& plain_data_type):
        Database<MyZP>(size, sparsity, plain_data_type ){}

// Encrypted database
EncryptedDatabase::EncryptedDatabase(int size, int sparsity, EncryptedDataType& encrypted_data_type):
        Database<HelibNumber>(size, sparsity, encrypted_data_type){}
