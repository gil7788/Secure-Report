//
// Created by gil on 29/04/20.
//

#include <zp.h>

#include "Server.h"


// Regular server
GenericServer::GenericServer(int size, int sparsity, GenericPlainDataType& plain_data_type):
        Server<GenericZP>(size, sparsity, plain_data_type ){}

// Encrypted server
EncryptedServer::EncryptedServer(int size, int sparsity, EncryptedDataTypeFromParameters& encrypted_data_type):
        Server<SimplifiedHelibNumber>(size, sparsity, encrypted_data_type){}
