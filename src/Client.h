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

template <typename DataType>
class EncryptedSecureReportQuery {
public:
    DataType _encrypted_lookup_value;
    DataType (*_isMatch)(DataType&, DataType&, int);

    EncryptedSecureReportQuery(DataType encrypted_lookup_value, DataType (*isMatch)(DataType&, DataType&, int)):
            _encrypted_lookup_value(encrypted_lookup_value),
            _isMatch{isMatch} {}
};

template <typename DataType>
class SecureReportQuery{
private:
    int _lookup_value;
    DataType (*_isMatch)(DataType&, DataType&, int);

public:
    SecureReportQuery(int lookup_value, DataType (*isMatch)(DataType&, DataType&, int)):
            _lookup_value{lookup_value},
            _isMatch{isMatch} {}

    int get_lookup_value() {
        return _lookup_value;
    }

    EncryptedSecureReportQuery<DataType> encrypt() {
        DataType encrypted_lookup_value = DataType::static_from_int(_lookup_value);
        return EncryptedSecureReportQuery<DataType>{encrypted_lookup_value, _isMatch};
    }
};

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
        for_each(plain_input.begin(), plain_input.end(),
                 [&](int entry){encrypted_input.push_back(DataType::static_from_int(entry)); });
        return encrypted_input;
    }

    EncryptedSecureReportQuery<DataType> query_server(SecureReportQuery<DataType> plain_query) {
        EncryptedSecureReportQuery<DataType> encrypted_query = plain_query.encrypt();

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

#endif //SECURE_REPORT_CLIENT_H
