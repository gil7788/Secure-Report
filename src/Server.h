//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>
#include "Config.h"

#include "Config.h"
#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"
#include "FHEUtils.h"
#include "Client.h"
#include "SimplifiedHelibNumber.h"

template <class EncryptedQuery, typename DataType>
class Server {
public:
    int _size;
    int _sparsity;
    FHEDatabase<EncryptedQuery, DataType> _fhe_database;
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

    bool upload(std::vector<DataType>& data) {
        connect_database();
        if(not _is_connected){
            _io.output("Failed to connect_database to database \n", constants::OUTPUT_LEVELS::ERROR);
        }

        bool database_built = _fhe_database.build_database_table(data);
        if(!database_built) {
            _io.output("Failed to build database \n", constants::OUTPUT_LEVELS::ERROR);
            return false;
        }
        return database_built;
    }

    std::vector<DataType> send_matches_indices_to_client(EncryptedQuery& encrypted_query,
                                                         TrustedThirdParty& trusted_third_party) {
        // Query database
        std::vector<DataType> matches_indicators {_fhe_database.evaluate_matches_indicators(encrypted_query)};

        // Encode encrypted data
        std::vector<DataType> encoded_encrypted_matches {_fhe_database.fhe_encode(matches_indicators, trusted_third_party)};

        return encoded_encrypted_matches;
    }

    void initialize() {
        _data_type.initialize();
    };
};


#endif //SECURE_REPORT_DATABASE_H
