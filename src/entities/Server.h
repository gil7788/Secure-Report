//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>

#include "../Config.h"
#include "../infrastructure/file_system/FHEDatabase.h"
#include "../algorithms/fully_homomorphic/Context.h"
#include "../algorithms/fully_homomorphic/Comparators.h"
#include "Client.h"
#include "../algorithms/fully_homomorphic/SimplifiedHelibNumber.h"
#include "../algorithms/hash_functions/HashFunctionsFamily.h"
#include "Queries.h"

// @TODO
// 1. Re-divide private, public functions
//  TrustedThirdParty initialize_public_server() in Server and SecureReportServer is public,
//  but in SecureBatchRetrievalServer is private

/**
 * This class implement virtual Server API for retrieval protocol SecureRetrievalProtocol.
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class Server {
public:
    int _database_size;
    int _number_of_matches;
    InputOutput _io;
    FHEDatabase<Number> _fhe_database;
    bool _is_connected = false;

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
    bool upload(std::vector<Number>& data) {
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
    vector<Number> send_matches_indices_to_client(const unique_ptr<TrustedThirdParty>& trusted_third_party) {
        vector<Number> matches_indicators {evaluate_matches()};
        vector<Number> encoded_encrypted_matches {encode(matches_indicators, trusted_third_party)};
        return encoded_encrypted_matches;
    }

    /**
     * Intialize TrustedThirdParty by processing pre-processed data
     * @return TrustedThirdParty with pre-processed data
     */
    virtual unique_ptr<TrustedThirdParty> initialize() {
        auto public_server = initialize_public_server();
        construct_disjunct_matrices(public_server);
        return public_server;
    };

    /**
     * Evaluate encrypted matches, base on EncryptedQuery provided in Server::initialize_query
     * @return Vector of encrypted matches indicators (vector of encrypted 0 or 1 values)
     */
    virtual vector<Number> evaluate_matches() = 0;

    virtual void initialize_query(EncryptedQuery<Number>& query) = 0;

    /**
     * Encode encrypted matches (see Server::evaluate_matches)
     * @param matches_indicators Vector of encrypted matches indicators (vector of encrypted 0 or 1 values)
     * @param public_server TrustedThirdParty
     * @return Encrypted and encoded vector of matches
     */
    vector<Number> encode(vector<Number>& matches_indicators, const unique_ptr<TrustedThirdParty>& public_server) {
        MatrixXi disjunct_matrix{get_disjunct_matrix(public_server).get_sketch()};
        std::vector<Number> out(disjunct_matrix.rows(), Number(0));

        for (int i_input = 0; i_input < _database_size; ++i_input) {
            Number x = matches_indicators[i_input];

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
    void construct_disjunct_matrices(unique_ptr<TrustedThirdParty>& public_server) {
        public_server->construct_sketch_encoders();
    }

    virtual unique_ptr<TrustedThirdParty> initialize_public_server() = 0;

    /**
     * Retrieve disjunct matrix from TrustedThirdParty according to EncryptedQuery.
     * @param public_server TrustedThirdParty
     * @return Disjunct matrix encoder
     */
    virtual SketchEncoder get_disjunct_matrix(const unique_ptr<TrustedThirdParty>& public_server) = 0;
};

/**
* This class implement Secure Report Server.
* @tparam Number Plain/Encrypted Fully Homomorphic number template,
* plausible templates inherit from VirtualContext.
*/
template <typename Number>
class SecureReportServer: public Server<Number> {
private:
    EncryptedSecureReportQuery<Number> _query;
public:
    SecureReportServer(int size, int sparsity):
            Server<Number>(size, sparsity) {}

    unique_ptr<TrustedThirdParty> initialize() {
        auto public_server = Server<Number>::initialize();
        return public_server;
    }

    /**
     * Initialize EncryptedSecureReportQuery sent from Client.
     * @param query EncryptedSecureReportQuery sent from Client
     */
    void initialize_query(EncryptedQuery<Number>& query) override {
        const EncryptedSecureReportQuery<Number> &casted_query = static_cast<const EncryptedSecureReportQuery<Number> &>(query);
        _query = casted_query;
    }

    vector<Number> evaluate_matches() {
        auto vectorized_database = Server<Number>::_fhe_database.table_to_vector();
        auto matches = evaluate_is_match_on_database(vectorized_database);
        return matches;
    }

    unique_ptr<TrustedThirdParty> initialize_public_server() {
        auto number_of_matches = Server<Number>::_number_of_matches;
        vector<int> sparsities{number_of_matches};
        unique_ptr<TrustedThirdParty> public_server (new TrustedThirdParty);

        public_server->initialize(Server<Number>::_database_size, sparsities);
        return public_server;
    }

    SketchEncoder get_disjunct_matrix(const unique_ptr<TrustedThirdParty>& public_server) {
//        Get first and only sketch encoder
        auto disjunct_matrix = public_server->get_encoder_by_index(0);
        return disjunct_matrix;
    }

private:
    EncryptedSecureReportQuery<Number> get_query() {
        return _query;
    }

    vector<Number> evaluate_is_match_on_database(vector<Number>& database) {
        vector<Number> isMatch_indicator;
        for(auto& element: database){
            auto element_is_match_indicator = evaluate_is_match_on_element(element);
            isMatch_indicator.push_back(element_is_match_indicator);
        }
        return isMatch_indicator;
    }

    Number evaluate_is_match_on_element(Number& element) {
        auto encrypted_value = get_query()._encrypted_lookup_value;
        return get_query()._is_match(encrypted_value, element);
    }
};

/**
* This class implement Secure Batch Retrieval Server.
* @tparam Number Plain/Encrypted Fully Homomorphic number template,
* plausible templates inherit from VirtualContext.
*/
template <typename Number>
class SecureBatchRetrievalServer: public Server<Number>{
private:
    vector<std::unique_ptr<HashFunctionFamily>> _hash_function_familys;
    int _query_index = 0;
public:
    EncryptedMultipleBatchRetrievalQuery<Number> _query;

public:
    SecureBatchRetrievalServer(int size, int sparsity):
    Server<Number>(size, sparsity) {}

    unique_ptr<TrustedThirdParty> initialize() {
        auto public_server = Server<Number>::initialize();
        sample_all_hash_mappings();
        return public_server;
    };

    /**
     * Initialize EncryptedMultipleBatchRetrievalQuery sent from Client.
     * @param query EncryptedMultipleBatchRetrievalQuery sent from Client
     */
    void initialize_query(EncryptedQuery<Number>& query) override {
        const EncryptedMultipleBatchRetrievalQuery<Number> &casted_query = static_cast<const EncryptedMultipleBatchRetrievalQuery<Number> &>(query);
        _query = casted_query;
    }

    vector<Number> evaluate_matches() {
        auto database = Server<Number>::_fhe_database.table_to_vector();
        auto batch_size = _query._batch_size;
        int hash_function_index = ceil(log2(batch_size));
        auto is_match_indicators = evaluate_is_match_on_database(database, hash_function_index);
        return is_match_indicators;
    }

    SketchEncoder get_disjunct_matrix(const unique_ptr<TrustedThirdParty>& public_server) {
        auto batch_size = _query._batch_size;
        auto matrix_index = ceil(log2(batch_size));
        auto disjunct_matrix = public_server->get_encoder_by_index(matrix_index);
        return disjunct_matrix;
    }

private:
    EncryptedMultipleBatchRetrievalQuery<Number> get_query() {
        return _query;
    }

    unique_ptr<TrustedThirdParty> initialize_public_server() {
        auto number_of_matches_word_length = ceil(log2(Server<Number>::_number_of_matches));
        auto database = Server<Number>::_database_size;
        vector<int> matrices_sparsities;

        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            auto sparsity_at_i = pow(2, i);
            matrices_sparsities.push_back(sparsity_at_i);
        }

        unique_ptr<TrustedThirdParty> public_server(new TrustedThirdParty);
        public_server->initialize(database, matrices_sparsities);
        return public_server;
    }

private:
    void sample_all_hash_mappings() {
        int number_of_matches_word_length = ceil(log2(Server<Number>::_number_of_matches));
        for (int i = 0; i <= number_of_matches_word_length; ++i) {
            _hash_function_familys.push_back(move(split_database_to_batches(i)));
        }
    }

    std::unique_ptr<HashFunctionFamily> split_database_to_batches(int number_of_batches_word_length) {
        int domain_word_size = ceil(log2(Server<Number>::_database_size));
        std::unique_ptr<PolynomialHashFunctionsFamily> hash_function(new PolynomialHashFunctionsFamily());

        if(number_of_batches_word_length == 0) {
            vector<int> batches_split(Server<Number>::_database_size, 0);
        }
        else {
            auto independence = ceil(log2(Server<Number>::_database_size));
            hash_function->initialize(domain_word_size, number_of_batches_word_length, independence);
            hash_function->build();
            hash_function->evaluate_all_domain();
        }
        return hash_function;
    }

private:
    vector<Number> evaluate_is_match_on_database(vector<Number>& database, int hash_function_index) {
        vector<Number> is_match_indicator;
        auto batch_query = get_next_query();
        for(int element_index = 0; element_index <  database.size(); ++element_index) {
            auto element_is_match_indicator = evaluate_is_match_on_element(database[element_index], element_index, hash_function_index, batch_query);
            is_match_indicator.push_back(element_is_match_indicator);
        }
        return is_match_indicator;
    }

    Number evaluate_is_match_on_element(Number element, int element_index, int hash_function_index, EncryptedSingleBatchRetrievalQuery<Number> batch_query) {
        if(_hash_function_familys[hash_function_index]->get_evaluated_value(element_index) == batch_query._batch_index) {
            auto encrypted_value = batch_query._encrypted_lookup_value;
            return batch_query._is_match(encrypted_value, element);
        }
        else {
            return Number(0);
        }
    }

    EncryptedSingleBatchRetrievalQuery<Number> get_next_query() {
        auto batch_query = get_query().get_next_batch_query();
       return batch_query;
    }
};


#endif //SECURE_REPORT_DATABASE_H
