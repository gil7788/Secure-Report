//
// Created by gil on 20.11.2020.
//

#ifndef SECURE_PROTOCOL_H
#define SECURE_PROTOCOL_H

#include <iostream>
#include "VectorUtils.h"
#include "Client.h"
#include "Server.h"


using namespace std;

template <typename DataType>
class SecureRetrievalProtocol {
public:
    Server<DataType>& _server;
    Client<DataType>& _client;
    TrustedThirdParty _public_server;

    SecureRetrievalProtocol(Server<DataType>& server, Client<DataType>& client):
            _server{server},
            _client{client} {}

    void initialize() {
        initialize_client();
        initialize_server();
        _public_server = get_server().construct_public_server();
    }

    bool upload(vector<int>& plain_data, int comparison_value) {
        vector<int> indices = VectorUtils::get_matches_std_vector(plain_data, comparison_value);

        string s = VectorUtils::std_vector_to_string(indices);
        std::cout << "Selected indices: " << s << std::endl;

        // Client
        auto encrypted_data = _client.upload_data_to_server(plain_data);

        // Server
        bool uploaded_successful = _server.upload(encrypted_data);
        return uploaded_successful;
    }

//    virtual void retrieve_all_matches();

    void query(unique_ptr<PlainQuery<DataType>> query) {
        SecureRetrievalProtocol<DataType>::get_client().initialize_query(move(query));
        auto encrypted_query = SecureRetrievalProtocol<DataType>::get_client().encrypt_query();
        SecureRetrievalProtocol<DataType>::get_server().initialize_query(move(encrypted_query));
    }

    std::vector<int> retrieve() {
        // Server
        std::vector<DataType> encrypted_matches_indices = _server.send_matches_indices_to_client(_public_server);

        //Client
        std::vector<int> matches_indices = _client.retrieve_matches_indices(encrypted_matches_indices, _public_server);
        return matches_indices;
    }

protected:
    virtual void initialize_client() = 0;

    virtual void initialize_server() = 0;

    virtual Server<DataType>& get_server() {
        return _server;
    }

    virtual Client<DataType>& get_client() {
        return _client;
    }
};

template <typename DataType>
class SecureBatchRetrievalProtocol: public SecureRetrievalProtocol<DataType> {
private:
    int _number_of_batches;

public:
    SecureBatchRetrievalProtocol(Server<DataType>& server, Client<DataType>& client):
            SecureRetrievalProtocol<DataType>(server, client) {}

    vector<int> retrieve_batch(int batch_index) {
//        TODO: fill
        vector<int> temp;
        return temp;
    }

    void retrieve_all_matches() {
        for(auto batch_index: range(_number_of_batches)) {
            retrieve_batch(batch_index);
        }
    }

    void initialize_server() {
        SecureRetrievalProtocol<DataType>::get_server().initialize();
    }

    void initialize_client() {
        SecureRetrievalProtocol<DataType>::get_client().initialize();
    }

    void run(int lookup_value,
             int batch_size,
             int batch_index,
             int database_size,
             int number_of_matches,
             DataType (*isMatch)(DataType&, DataType&),
             std::vector<int>& plain_data) {
        bool uploaded_successful = SecureRetrievalProtocol<DataType>::upload(plain_data, lookup_value);

        if(! uploaded_successful) {
            std::cout << "Failed to upload data\n";
            return;
        }

        SecureRetrievalProtocol<DataType>::initialize();

        SecureReportServer<DataType>::get_client().initialize_query(lookup_value, batch_size, batch_index, database_size, number_of_matches, isMatch);
        auto encrypted_query = SecureReportServer<DataType>::get_client().encrypt_query();
        SecureReportServer<DataType>::get_server().initialize_query(encrypted_query);
        // Client
        std::vector<int> matches_indices = SecureReportClient<DataType>::retrieve();

        string result_string = VectorUtils::std_vector_to_string(matches_indices);
        std::cout << "Result: " << result_string << std::endl;
    }
};

template <typename DataType>
class SecureReportProtocol: public SecureRetrievalProtocol<DataType> {

};

#endif //SECURE_PROTOCOL_H
