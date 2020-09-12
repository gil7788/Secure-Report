//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>

#include "Config.h"
#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"
#include "FHEUtils.h"
#include "Client.h"
#include "SimplifiedHelibNumber.h"
#include "HashFunctions/HashFunctionsFamily.h"
#include "Queries.h"

template <typename DataType>
class Server {
public:
    int _database_size;
    int _number_of_matches;
    FHEDatabase<DataType> _fhe_database;
    DatabaseDataType& _data_type;
    InputOutput _io = InputOutput(constants::OUTPUT_TO_CONSOLE, constants::OUTPUT_FILE_PATH, constants::OUTPUT_LEVEL);
    bool _is_connected = false;

    Server(int size, int sparsity, DatabaseDataType& data_type):
            _database_size{size},
            _number_of_matches{sparsity},
            _fhe_database(_database_size, _io), _data_type(data_type) {}

    virtual void connect_database() {
        if(_is_connected)
            return;

        // Connect to plain database
        _is_connected = _fhe_database.connect();
        if(!_is_connected) {
            _io.output("Failed to connect_database to database\n", constants::OUTPUT_LEVELS::ERROR);
        }
    }

    virtual bool upload(std::vector<DataType>& data) {
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

    virtual vector<DataType> send_matches_indices_to_client(TrustedThirdParty& trusted_third_party) {

        // Query database
        vector<DataType> matches_indicators {evaluate_matches()};

        // Encode encrypted data
        vector<DataType> encoded_encrypted_matches {encode(matches_indicators, trusted_third_party)};

        return encoded_encrypted_matches;
    }

    virtual TrustedThirdParty initialize() {
        construct_public_server();
        auto public_server = construct_public_server();
        construct_disjunct_matrices(public_server);
        return public_server;
    };

    virtual vector<DataType> evaluate_matches() = 0;

    virtual vector<DataType> encode(vector<DataType>& matches_indicators, TrustedThirdParty& public_server) {
        MatrixXi disjunct_matrix{get_disjunct_matrix(public_server).get_sketch()};
        std::vector<DataType> out(disjunct_matrix.rows(), DataType(0));

        for (int i_input = 0; i_input < _database_size; ++i_input) {
            DataType x = matches_indicators[i_input];

            for (int i_out = 0; i_out < disjunct_matrix.rows(); ++i_out) {
                if (disjunct_matrix(i_out, i_input) == 1)
                    out[i_out] += x;
            }
        }
        return out;
    }

    virtual void construct_disjunct_matrices(TrustedThirdParty& public_server) {
        auto encoders = public_server._encoders;
        for(auto& encoder: encoders){
            encoder.construct_sketch_matrix();
        }
    }

    virtual TrustedThirdParty construct_public_server() = 0;

    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) = 0;
};

template <typename DataType>
class SecureReportServer: public Server<DataType> {
EncryptedSecureReportQuery<DataType> _current_query;

public:
    SecureReportServer(int size, int sparsity, DatabaseDataType& data_type):
            Server<DataType>(size, sparsity, data_type) {}

    virtual TrustedThirdParty initialize() {
        return Server<DataType>::initialize();
    }

    virtual vector<DataType> evaluate_matches() {
        auto vectorized_database = Server<DataType>::_fhe_database.table_to_vector();
        auto matches = evaluate_is_match_on_database(vectorized_database);
        return matches;
    }

    virtual TrustedThirdParty construct_public_server() {
        auto number_of_matches = Server<DataType>::_number_of_matches;
        auto public_server = TrustedThirdParty(Server<DataType>::_database_size, number_of_matches);
        return public_server;
    }

    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto disjunct_matrix = public_server.get_matrix_by_index(0);
        return disjunct_matrix;
    }

    void initialize_query(EncryptedSecureReportQuery<DataType>& query) {
        _current_query.initialize(query._encrypted_lookup_value,
                                  query._isMatch);
    }
private:
    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database) {
        vector<DataType> isMatch_indicator;
        for(auto& element: database){
            auto element_isMatch_indicator = evaluate_is_match_on_element(element);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType element) {
        return _current_query._isMatch(_current_query._encrypted_lookup_value, element);
    }
};

template <typename DataType>
class SecureBatchRetrievalServer: public Server<DataType>{
private:
vector<vector<int>> _batches_splits;
vector<MatrixXi> _disjunct_matrices;
EncryptedSecureBatchRetrievalQuery<DataType> _current_query;

public:
    SecureBatchRetrievalServer(int size, int sparsity, DatabaseDataType& data_type):
            Server<DataType>(size, sparsity, data_type) {}

    virtual TrustedThirdParty initialize() {
        auto public_server = Server<DataType>::initialize();
        sample_all_hash_mappings();
        return public_server;
    };

    virtual vector<DataType> evaluate_matches() {
        auto database = Server<DataType>::_fhe_database.table_to_vector();
        int batch_size_word_length = ceil(log2(_current_query._batch_size));
        auto batches_split = _batches_splits[batch_size_word_length];
        auto is_match_indicators = evaluate_is_match_on_database(database, batches_split);
        return is_match_indicators;
    }

    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto matrix_index = ceil(log2(_current_query._batch_size));
        auto disjunct_matrix = public_server.get_matrix_by_index(matrix_index);
        return disjunct_matrix;
    }

    void initialize_query(EncryptedSecureBatchRetrievalQuery<DataType>& query) {
        _current_query.initialize(query._encrypted_lookup_value,
                                  query._batch_index,
                                  query._batch_size,
                                  query._isMatch);
    }

private:
    virtual TrustedThirdParty construct_public_server() {
        auto number_of_matches_word_length = ceil(log2(Server<DataType>::_number_of_matches));
        auto database = Server<DataType>::_database_size;
        vector<int> matrices_sparsities;

        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            auto sparsity_at_i = pow(2, i);
            matrices_sparsities.push_back(sparsity_at_i);
        }

        auto public_server = TrustedThirdParty(database, matrices_sparsities);
        return public_server;
    }
private:
    void sample_all_hash_mappings() {
        int number_of_matches_word_length = ceil(log2(Server<DataType>::_number_of_matches));
        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            vector<int> split_i = split_database_to_batches(i);
            _batches_splits.push_back(split_i);
        }
    }

    vector<int> split_database_to_batches(int number_of_batches_word_length) {
        int domain_word_size = log2(Server<DataType>::_database_size);


        if(number_of_batches_word_length == 0) {
            vector<int> batches_split(Server<DataType>::_database_size, 0);
            return batches_split;
        }
        else {
            auto independence = ceil(log2(Server<DataType>::_database_size));
            PolynomialHashFunctionsFamily hash_function(independence);
            hash_function.initialize(domain_word_size, number_of_batches_word_length);
            vector<int> batches_split = hash_function.evaluate_all_domain();
            return batches_split;
        }
    }

private:
    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database,
                                                   vector<int>& batches_split) {
        vector<DataType> isMatch_indicator;
        for(int element_index = 0; element_index <  database.size(); ++element_index) {
            auto element_isMatch_indicator = evaluate_is_match_on_element(database[element_index], element_index, batches_split);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType element, int element_index, vector<int>& batches_split) {
        if(batches_split[element_index] == _current_query._batch_index) {
            return _current_query._isMatch(_current_query._encrypted_lookup_value, element);
        }
        else {
            return DataType(0);
        }
    }
};


#endif //SECURE_REPORT_DATABASE_H
