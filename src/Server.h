//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>
#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

#include "Config.h"
#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"
#include "FHEUtils.h"
#include "Client.h"

template <typename DataType>
class Server {
public:
    int _size;
    int _sparsity;
    FHEDatabase<DataType> _fhe_database;
    DatabaseDataType& _data_type;
    InputOutput _io = InputOutput(constants::OUTPUT_TO_CONSOLE, constants::OUTPUT_FILE_PATH, constants::OUTPUT_LEVEL);
    bool _is_connected = false;

    Server(int size, int sparsity, DatabaseDataType& data_type):
            _size{size},
            _sparsity{sparsity},
            _fhe_database(_size, _sparsity, _io),
            _data_type(data_type){}

    void connect_database() {
        if(_is_connected)
            return;

        // Connect to plain database
        _is_connected = _fhe_database.connect();
        if(!_is_connected) {
            _io.output("Failed to connect_database to database\n", constants::OUTPUT_LEVELS::ERROR);
        }
    }


    bool upload(std::vector<int>& data) {
        connect_database();
        if(not _is_connected){
            _io.output("Failed to connect_database to database \n", constants::OUTPUT_LEVELS::ERROR);
        }

        // TODO build encrypted database - use encrypt_input method for it
        bool database_built = _fhe_database.build_database_table(data);
        if(!database_built) {
            _io.output("Failed to build database \n", constants::OUTPUT_LEVELS::ERROR);
            return false;
        }
    }

    std::vector<DataType> send_matches_indices_to_client(EncryptedSecureReportQuery<DataType>& encrypted_query,
                                                         TrustedThirdParty& trusted_third_party) {
        // Query database
        std::vector<DataType> matches_indicators {_fhe_database.evaluate_matches_indicators(encrypted_query)};

        // Encode encrypted data
        std::vector<DataType> encoded_encrypted_matches {_fhe_database.fhe_encode(matches_indicators, trusted_third_party)};

        return encoded_encrypted_matches;
    }

    void initialize() {
        _data_type.initialize_data_type();
    };
};


class PlainServer: public Server<MyZP> {
public:

    PlainServer(int size, int sparsity, PlainDataType& plain_data_type);

};

class EncryptedServer: public Server<HelibNumber> {
public:

    EncryptedServer(int size, int sparsity, EncryptedDataType& data_type);

};
#endif //SECURE_REPORT_DATABASE_H
