//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>

#include "../Config.h"
#include "../infrastructure/file_system/FHEDatabase.h"
#include "../algorithms/fully_homomorphic/FHEDatabaseConfig.h"
#include "../algorithms/fully_homomorphic/FHEUtils.h"
#include "Client.h"
#include "../algorithms/fully_homomorphic/SimplifiedHelibNumber.h"
#include "../algorithms/hash_functions/HashFunctionsFamily.h"
#include "Queries.h"

// @TODO
// 1. Re-divide private, public functions
//  TrustedThirdParty construct_public_server() in Server and SecureReportServer is public,
//  but in SecureBatchRetrievalServer is private

/**
 * This class implement virtual Server API for retrieval protocol SecureRetrievalProtocol.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class Server {
public:
    int _database_size;
    int _number_of_matches;
    InputOutput _io;
    FHEDatabase<DataType> _fhe_database;
    bool _is_connected = false;
    unique_ptr<EncryptedQuery<DataType>> _query;

    Server(int size, int sparsity):
            _database_size{size},
            _number_of_matches{sparsity},
            _io(constants::OUTPUT_TO_CONSOLE, constants::OUTPUT_FILE_PATH, constants::OUTPUT_LEVEL),
            _fhe_database(size, _io) {}

    /**
     * Connect to database
     */
    void connect_database() {
        if(_is_connected)
            return;

        // Connect to plain database
        _is_connected = _fhe_database.connect();
        if(!_is_connected) {
            _io.output("Failed to connect_database to database\n", constants::OUTPUT_LEVELS::ERROR);
        }
    }

    /**
     * Upload Encrypted data to database
     * @param data Encrypted data
     * @return Upload success
     */
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

    /**
     * Evaluate matches (encrypted and encoded) as a result to EncryptedQuery sent by Client.
     * @param trusted_third_party TrustedThirdParty
     * @return Encrypted and encoded matches
     */
    vector<DataType> send_matches_indices_to_client(TrustedThirdParty& trusted_third_party) {
        vector<DataType> matches_indicators {evaluate_matches()};
        vector<DataType> encoded_encrypted_matches {encode(matches_indicators, trusted_third_party)};
        return encoded_encrypted_matches;
    }

    /**
     * Intialize TrustedThirdParty by processing pre-processed data
     * @return TrustedThirdParty with pre-processed data
     */
    virtual TrustedThirdParty initialize() {
        auto public_server = construct_public_server();
        construct_disjunct_matrices(public_server);
        return public_server;
    };

    /**
     * Initialize EncryptedQuery sent from Client.
     * @param query EncryptedQuery sent from Client
     */
    void initialize_query(unique_ptr<EncryptedQuery<DataType>> query) {
        _query = move(query);
    }

    /**
     * Evaluate encrypted matches, base on EncryptedQuery provided in Server::initialize_query
     * @return Vector of encrypted matches indicators (vector of encrypted 0 or 1 values)
     */
    virtual vector<DataType> evaluate_matches() = 0;

    /**
     * Encode encrypted matches (see Server::evaluate_matches)
     * @param matches_indicators Vector of encrypted matches indicators (vector of encrypted 0 or 1 values)
     * @param public_server TrustedThirdParty
     * @return Encrypted and encoded vector of matches
     */
    vector<DataType> encode(vector<DataType>& matches_indicators, TrustedThirdParty& public_server) {
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

    /**
     * Construct disjunct matrices in the public server
     * @param public_server TrustedThirdParty
     */
    void construct_disjunct_matrices(TrustedThirdParty& public_server) {
        public_server.construct_sketch_matrices();
    }

    virtual TrustedThirdParty construct_public_server() = 0;

    /**
     * Retrieve disjunct matrix from TrustedThirdParty according to EncryptedQuery.
     * @param public_server TrustedThirdParty
     * @return Disjunct matrix encoder
     */
    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) = 0;
};

/**
* This class implement Secure Report Server.
* @tparam DataType Plain/Encrypted Fully Homomorphic number template,
* plausible templates inherit from DatabaseDataType.
*/
template <typename DataType>
class SecureReportServer: public Server<DataType> {
public:
    SecureReportServer(int size, int sparsity):
            Server<DataType>(size, sparsity) {}

    TrustedThirdParty initialize() {
        return Server<DataType>::initialize();
    }

    vector<DataType> evaluate_matches() {
        auto vectorized_database = Server<DataType>::_fhe_database.table_to_vector();
        auto matches = evaluate_is_match_on_database(vectorized_database);
        return matches;
    }

    TrustedThirdParty construct_public_server() {
        auto number_of_matches = Server<DataType>::_number_of_matches;
        vector<int> sparsities{number_of_matches};
        TrustedThirdParty public_server;

        public_server.initialize(Server<DataType>::_database_size, sparsities);
        return public_server;
    }

    SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto disjunct_matrix = public_server.get_matrix_by_index(0);
        return disjunct_matrix;
    }

private:
    EncryptedSecureReportQuery<DataType> get_query() {
        EncryptedSecureReportQuery<DataType>* query =
                dynamic_cast<EncryptedSecureReportQuery<DataType>*> (Server<DataType>::_query.get());
        if(query == nullptr) {
            cerr << "Failed to cast Query abstract class to derived class\n";
        }
        return *query;
    }

    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database) {
        vector<DataType> isMatch_indicator;
        for(auto& element: database){
            auto element_isMatch_indicator = evaluate_is_match_on_element(element);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType& element) {
        auto encrypted_value = get_query()._encrypted_lookup_value;
        return get_query()._isMatch(encrypted_value, element);
    }
};

/**
* This class implement Secure Batch Retrieval Server.
* @tparam DataType Plain/Encrypted Fully Homomorphic number template,
* plausible templates inherit from DatabaseDataType.
*/
template <typename DataType>
class SecureBatchRetrievalServer: public Server<DataType>{
private:
vector<std::unique_ptr<HashFunctionFamily>> _hash_function_familys;

public:
    SecureBatchRetrievalServer(int size, int sparsity):
    Server<DataType>(size, sparsity) {}

    TrustedThirdParty initialize() {
        auto public_server = Server<DataType>::initialize();
        sample_all_hash_mappings();
        return public_server;
    };

    vector<DataType> evaluate_matches() {
        auto database = Server<DataType>::_fhe_database.table_to_vector();
        int hash_function_index = ceil(log2(get_query()._batch_size));
        auto is_match_indicators = evaluate_is_match_on_database(database, hash_function_index);
        return is_match_indicators;
    }

    SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto matrix_index = ceil(log2(get_query()._batch_size));
        auto disjunct_matrix = public_server.get_matrix_by_index(matrix_index);
        return disjunct_matrix;
    }

private:
    EncryptedSecureBatchRetrievalQuery<DataType> get_query() {
        EncryptedSecureBatchRetrievalQuery<DataType>* query =
                dynamic_cast<EncryptedSecureBatchRetrievalQuery<DataType>*> (Server<DataType>::_query.get());
        if(query == nullptr) {
            cerr << "Failed to cast Query abstract class to derived class\n";
        }
        return *query;
    }

    TrustedThirdParty construct_public_server() {
        auto number_of_matches_word_length = ceil(log2(Server<DataType>::_number_of_matches));
        auto database = Server<DataType>::_database_size;
        vector<int> matrices_sparsities;

        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            auto sparsity_at_i = pow(2, i);
            matrices_sparsities.push_back(sparsity_at_i);
        }

        TrustedThirdParty public_server;
        public_server.initialize(database, matrices_sparsities);
        return public_server;
    }

private:
    void sample_all_hash_mappings() {
        int number_of_matches_word_length = ceil(log2(Server<DataType>::_number_of_matches));
        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            _hash_function_familys.push_back(move(split_database_to_batches(i)));
        }
    }

    std::unique_ptr<HashFunctionFamily> split_database_to_batches(int number_of_batches_word_length) {
        int domain_word_size = log2(Server<DataType>::_database_size);
        std::unique_ptr<PolynomialHashFunctionsFamily> hash_function(new PolynomialHashFunctionsFamily());

        if(number_of_batches_word_length == 0) {
            vector<int> batches_split(Server<DataType>::_database_size, 0);
        }
        else {
            auto independence = ceil(log2(Server<DataType>::_database_size));
            hash_function->initialize(domain_word_size, number_of_batches_word_length, independence);
            hash_function->build();
            hash_function->evaluate_all_domain();
        }
        return hash_function;
    }

private:
    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database, int hash_function_index) {
        vector<DataType> isMatch_indicator;
        for(int element_index = 0; element_index <  database.size(); ++element_index) {
            auto element_isMatch_indicator = evaluate_is_match_on_element(database[element_index], element_index, hash_function_index);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType element, int element_index, int hash_function_index) {
        if(_hash_function_familys[hash_function_index]->get_evaluated_value(element_index) == get_query()._batch_index) {
            auto encrypted_value = get_query()._encrypted_lookup_value;
            return get_query()._isMatch(encrypted_value, element);
        }
        else {
            return DataType(0);
        }
    }
};


#endif //SECURE_REPORT_DATABASE_H
