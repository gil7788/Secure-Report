//
// Created by gil on 02/05/20.
//

#ifndef SECURE_REPORT_CLIENT_H
#define SECURE_REPORT_CLIENT_H

#include <Eigen/Dense>

using Eigen::VectorXi;

#include <bits/unique_ptr.h>
#include "FHEDatabaseConfig.h"
#include "TrustedThirdParty.h"
#include "FileUtils.h"
#include "SimplifiedHelibNumber.h"
#include "Queries.h"


template <typename DataType>
class Client {
public:
    int _datase_size;
    int _maximal_number_of_matches_per_query;
    DatabaseDataType& _data_type;

    Client(int datase_size, int maximal_number_of_matches_per_query, DatabaseDataType& data_type):
            _datase_size{datase_size},
            _maximal_number_of_matches_per_query{maximal_number_of_matches_per_query},
            _data_type(data_type){}

    virtual bool initialize() {
        _data_type.initialize();
    }

    virtual std::vector<DataType> upload_data_to_server(std::vector<int>& data) {
        // Encrypt
        auto encrypted_data = encrypt_data(data);
        // Send to Server

        return encrypted_data;
    }

    virtual std::vector<DataType> encrypt_data(std::vector<int>& plain_input) {
        std::vector<DataType> encrypted_input;
        for(auto& entry: plain_input) {
            DataType encrypted_entry = DataType(entry);
            encrypted_input.push_back(encrypted_entry);
        }
        return encrypted_input;
    }

    virtual std::vector<int> retrieve_matches_indices(std::vector<DataType>&  encoded_encrypted_matches, TrustedThirdParty& trusted_third_party) {
        std::vector<int> encoded_matches = decrypt_encrypted_encoded_matches(encoded_encrypted_matches);

        // Decode encoded into decoded
        std::vector<int> matches = decode_encoded_matches(encoded_matches, trusted_third_party);

        return matches;
    }

private:
    virtual std::vector<int> decrypt_encrypted_encoded_matches(std::vector<DataType>& encrypted_encoded_matches) {
        std::vector<int> encoded_matches(encrypted_encoded_matches.size());
        for (int i_output = 0; i_output < encrypted_encoded_matches.size(); ++i_output) {
            encoded_matches[i_output] = encrypted_encoded_matches[i_output].to_int();
        }

        return encoded_matches;
    }

    virtual std::vector<int> decode_encoded_matches(std::vector<int>& encoded_matches,
            TrustedThirdParty& public_server) {
        /*
         * Description: Decode plain vector
         * */

        for (int i = 0; i < encoded_matches.size(); ++i) {
            (encoded_matches[i] > 0) ? encoded_matches[i] = 1 : encoded_matches[i] = 0;
        }

        Eigen::Map<Eigen::VectorXi> eigen_encoded_matches(&encoded_matches[0], encoded_matches.size());

        auto encoder = get_disjunct_matrix(public_server);
        VectorXi eigen_vector_matches = encoder.decode(eigen_encoded_matches);
        std::vector<int> matches = VectorUtils::eigen_vector_to_std_vector(eigen_vector_matches);

        return matches;
    }

    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) = 0;
};

template <typename DataType>
class SecureReportClient: public Client<DataType> {
    SecureReportQuery<DataType> _query;
public:
    SecureReportClient(int datase_size, int maximal_number_of_matches_per_query, DatabaseDataType& data_type):
            Client<DataType>(datase_size, maximal_number_of_matches_per_query, data_type) {}

    EncryptedSecureReportQuery<DataType> query_server() {
        EncryptedSecureReportQuery<DataType> encrypted_query = _query.encrypt();

        return encrypted_query;
    }

    void initialize_query(int lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        _query.initialize(lookup_value, isMatch);
    }

    EncryptedSecureReportQuery<DataType> encrypt_query() {
        return _query.encrypt();
    }

private:
    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto sketch_encoder = public_server.get_matrix_by_index(0);
        return sketch_encoder;
    }
};

template <typename DataType>
class SecureBatchRetrievalClient: public Client<DataType> {
SecureBatchRetrievalQuery<DataType> _query;
public:
    SecureBatchRetrievalClient(int datase_size, int maximal_number_of_matches_per_query, DatabaseDataType& data_type):
            Client<DataType>(datase_size, maximal_number_of_matches_per_query, data_type) {}

    EncryptedSecureBatchRetrievalQuery<DataType> query_server() {
        EncryptedSecureBatchRetrievalQuery<DataType> encrypted_query = _query.encrypt();

        return encrypted_query;
    }

    void initialize_query(int lookup_value, int batch_size, int batch_index, int database_size, int number_of_matches,
            DataType (*isMatch)(DataType&, DataType&)) {
        _query.initialize(lookup_value, batch_size, batch_index, database_size, number_of_matches, isMatch);
    }

    EncryptedSecureBatchRetrievalQuery<DataType> encrypt_query() {
        return _query.encrypt();
    }

private:
    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto encoder_index = ceil(log2(_query._batch_size));
        auto sketch_encoder = public_server.get_matrix_by_index(encoder_index);
        return sketch_encoder;
    }
};


#endif //SECURE_REPORT_CLIENT_H
