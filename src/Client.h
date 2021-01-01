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
    unique_ptr<DatabaseDataType>& _data_type;
    unique_ptr<PlainQuery<DataType>> _query;

    Client(unique_ptr<DatabaseDataType>& data_type):
            _data_type(data_type) {}

    bool initialize() {
        _data_type.get()->initialize();
    }

    void initialize_query(unique_ptr<PlainQuery<DataType>> query) {
        _query = move(query);
    }

    virtual std::vector<DataType> upload_data_to_server(std::vector<int>& data) {
        auto encrypted_data = encrypt_data(data);
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

    unique_ptr<EncryptedQuery<DataType>> encrypt_query() {
        return _query.get()->encrypt();
    }

    virtual std::vector<int> retrieve_matches_indices(std::vector<DataType>&  encoded_encrypted_matches, TrustedThirdParty& trusted_third_party) {
        std::vector<int> encoded_matches = decrypt_encrypted_encoded_matches(encoded_encrypted_matches);
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
        for (int i = 0; i < encoded_matches.size(); ++i) {
            (encoded_matches[i] > 0) ? encoded_matches[i] = 1 : encoded_matches[i] = 0;
        }

        Eigen::Map<Eigen::VectorXi> eigen_encoded_matches(&encoded_matches[0], encoded_matches.size());

        auto encoder = get_disjunct_matrix(public_server);
        VectorXi eigen_vector_matches = encoder.decode(eigen_encoded_matches);
        auto matches_long = VectorUtils::eigen_vector_to_std_vector(eigen_vector_matches);
        std::vector<int> matches(begin(matches_long), end(matches_long));

        return matches;
    }

    virtual SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) = 0;
};

template <typename DataType>
class SecureReportClient: public Client<DataType> {
public:
    SecureReportClient(unique_ptr<DatabaseDataType>& data_type):
            Client<DataType>(data_type) {}

private:
    SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto sketch_encoder = public_server.get_matrix_by_index(0);
        return sketch_encoder;
    }
};

template <typename DataType>
class SecureBatchRetrievalClient: public Client<DataType> {
SecureBatchRetrievalQuery<DataType> _query;
public:
    SecureBatchRetrievalClient(unique_ptr<DatabaseDataType>& data_type):
            Client<DataType>(data_type) {}

private:
    SketchEncoder get_disjunct_matrix(TrustedThirdParty& public_server) {
        auto encoder_index = ceil(log2(get_query()._batch_size));
        auto sketch_encoder = public_server.get_matrix_by_index(encoder_index);
        return sketch_encoder;
    }

    SecureBatchRetrievalQuery<DataType> get_query() {
        SecureBatchRetrievalQuery<DataType>* query =
                dynamic_cast<SecureBatchRetrievalQuery<DataType>*> (Client<DataType>::_query.get());
        if(query == nullptr) {
            cerr << "Failed to cast Query abstract class to derived class\n";
        }
        return *query;
    }
};


#endif //SECURE_REPORT_CLIENT_H
