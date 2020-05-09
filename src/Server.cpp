//
// Created by gil on 29/04/20.
//

#include <zp.h>

#include "Server.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

// Regular database
PlainServer::PlainServer(int size, int sparsity, PlainDataType& plain_data_type):
        Server<MyZP>(size, sparsity, plain_data_type ){}

// Encrypted database
EncryptedServer::EncryptedServer(int size, int sparsity, EncryptedDataType& encrypted_data_type):
        Server<HelibNumber>(size, sparsity, encrypted_data_type){}
