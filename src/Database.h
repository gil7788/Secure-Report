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

template <typename DataType>
class Database {
public:
    int _size;
    int _sparsity;
    FHEDatabase<DataType> _fhe_database;
    DatabaseDataType& _data_type;
    InputOutput _io = InputOutput(constants::OUTPUT_TO_CONSOLE, constants::OUTPUT_FILE_PATH, constants::OUTPUT_LEVEL);
    bool _is_connected = false;

    Database(int size, int sparsity, DatabaseDataType& data_type):
            _size{size},
            _sparsity{sparsity},
            _fhe_database(_size, _sparsity, _io),
            _data_type(data_type){}

    void connect() {
        if(_is_connected)
            return;

        // Connect to plain database
        _is_connected = _fhe_database.connect();
        if(!_is_connected) {
            _io.output("Failed to connect to database\n", constants::OUTPUT_LEVELS::ERROR);
        }
    }


    bool upload(VectorXi data) {
        connect();
        if(not _is_connected){
            _io.output("Failed to connect to database \n", constants::OUTPUT_LEVELS::ERROR);
        }

        // TODO build encrypted database - use encrypt_input method for it
        bool database_built = _fhe_database.build_database_table(data);
        if(!database_built) {
            _io.output("Failed to build database \n", constants::OUTPUT_LEVELS::ERROR);
            return false;
        }
    }

    std::unique_ptr<DataType[]> evaluate_matches_indices(std::unique_ptr<DataType>& lookup_value,
                                                    DataType (*isMatch)(std::unique_ptr<DataType>&,
                                                                        std::unique_ptr<DataType>&, int)) {
        // Query database
        std::unique_ptr<DataType[]> matches_indicators(_fhe_database.evaluate_matches_indicators(lookup_value, isMatch));

        // Encode encrypted data
        auto encoded_encrypted_matches(_fhe_database.fhe_encode(matches_indicators));

        return encoded_encrypted_matches;
    }

    VectorXi client_retrieve_matches_indices(std::unique_ptr<DataType[]>& encoded_encrypted_matches) {
        VectorXi encoded_matches = _fhe_database.fhe_decrypt(encoded_encrypted_matches);

        // Decode encoded into decoded
        VectorXi matches = _fhe_database.decode(encoded_matches);

        return matches;
    }

    void initialize_data_type() {
        _data_type.initialize_data_type();
    };
};


class RegularDatabase: public Database<MyZP> {
public:

    RegularDatabase(int size, int sparsity, PlainDataType& plain_data_type);

};

class EncryptedDatabase: public Database<HelibNumber> {
public:

    EncryptedDatabase(int size, int sparsity, EncryptedDataType& data_type);

};
#endif //SECURE_REPORT_DATABASE_H
