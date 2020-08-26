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

    bool initialize() {
        _data_type.initialize();
    }

    std::vector<DataType> upload_data_to_server(std::vector<int>& data) {
        // Encrypt
        auto encrypted_data = encrypt_data(data);
        // Send to Server

        return encrypted_data;
    }

    std::vector<DataType> encrypt_data(std::vector<int>& plain_input) {
        std::vector<DataType> encrypted_input;
        for(auto& entry: plain_input) {
            DataType encrypted_entry = DataType(entry);
            encrypted_input.push_back(encrypted_entry);
        }
        return encrypted_input;
    }

    template<class PlainQuery, class EncryptedQuery>
    EncryptedQuery query_server(PlainQuery plain_query) {
        EncryptedQuery encrypted_query = plain_query.encrypt();

        return encrypted_query;
    }

    std::vector<int> retrieve_matches_indices(std::vector<DataType>&  encoded_encrypted_matches, TrustedThirdParty& trusted_third_party) {
        std::vector<int> encoded_matches = decrypt_encrypted_encoded_matches(encoded_encrypted_matches);

        // Decode encoded into decoded
        std::vector<int> matches = decode_encoded_matches(encoded_matches, trusted_third_party);

        return matches;
    }

private:
    std::vector<int> decrypt_encrypted_encoded_matches(std::vector<DataType>& encrypted_encoded_matches) {
        std::vector<int> encoded_matches(encrypted_encoded_matches.size());
        for (int i_output = 0; i_output < encrypted_encoded_matches.size(); ++i_output) {
            encoded_matches[i_output] = encrypted_encoded_matches[i_output].to_int();
        }

        return encoded_matches;
    }

    std::vector<int> decode_encoded_matches(std::vector<int>& encoded_matches, TrustedThirdParty& trusted_third_party) {
        /*
         * Description: Decode plain vector
         * */

        for (int i = 0; i < encoded_matches.size(); ++i) {
            (encoded_matches[i] > 0) ? encoded_matches[i] = 1 : encoded_matches[i] = 0;
        }

        Eigen::Map<Eigen::VectorXi> eigen_encoded_matches(&encoded_matches[0], encoded_matches.size());

        VectorXi eigen_vector_matches = trusted_third_party._encoder.decode(eigen_encoded_matches);
        std::vector<int> matches = VectorUtils::eigen_vector_to_std_vector(eigen_vector_matches);

        return matches;
    }
};

class GenericClient: public Client<GenericZP> {
public:
    GenericClient(int size, int sparsity, GenericPlainDataType& plain_data_type);
};

class EncryptedClient: public Client<SimplifiedHelibNumber> {
public:
    EncryptedClient(int size, int sparsity, EncryptedDataTypeFromParameters& data_type);
};

#endif //SECURE_REPORT_CLIENT_H
