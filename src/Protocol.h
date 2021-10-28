//
// Created by gil on 20.11.2020.
//

#ifndef SECURE_PROTOCOL_H
#define SECURE_PROTOCOL_H

#include <iostream>
#include "utils/mathmatical/VectorUtils.h"
#include "Client.h"
#include "Server.h"


using namespace std;

// TODO:
//  1. Add Aproximate counting
//  2. Add Slackness

/**
 * This class implement virtual Secure Retrieval Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class SecureRetrievalProtocol {
public:
    Server<DataType>& _server;
    Client<DataType>& _client;
    TrustedThirdParty _public_server;

    SecureRetrievalProtocol(Server<DataType>& server, Client<DataType>& client):
            _server{server},
            _client{client} {}

    /**
     * Initialize protocol's entities: client, server and trusted third party
     */
    void initialize() {
        initialize_client();
        initialize_server();
        _public_server = get_server().construct_public_server();
    }
    /**
     * Client encrypt plain database data and upload encrypted data to the server.
     * @param plain_data
     * @return Upload successfully
     */
    bool upload(vector<int>& plain_data) {
        auto encrypted_data = _client.upload_data_to_server(plain_data);
        bool uploaded_successful = _server.upload(encrypted_data);
        return uploaded_successful;
    }

    /**
     * Client encrypt PlainQuery and send EncryptedQuery to Server, to proccess it and evaluate desired encrypted query.
     * To query Server, first Client should upload data to Server, by SecureRetrievalProtocol::upload.
     * @param query Client selected PlainQuery
     */
    void query(unique_ptr<PlainQuery<DataType>> query) {
        get_client().initialize_query(move(query));
        auto encrypted_query = get_client().encrypt_query();
        get_server().initialize_query(move(encrypted_query));
    }

    /**
     * Client retrieve output from server, evaluated based on EncryptedQuery (see SecureRetrievalProtocol::query).
     * Client evaluate plain desired result by decrypt and decode output from Server.
     * @return Matches indices
     */
    std::vector<int> retrieve() {
        std::vector<DataType> encrypted_matches_indices = _server.send_matches_indices_to_client(_public_server);

        std::vector<int> matches_indices = _client.retrieve_matches_indices(encrypted_matches_indices, _public_server);
        return matches_indices;
    }

protected:
    void initialize_client() {
        _client.initialize();
    }

    void initialize_server() {
        _server.initialize();
    }

    Server<DataType>& get_server() {
        return _server;
    }

    Client<DataType>& get_client() {
        return _client;
    }
};


/**
 * This class implement Secure Batch Retrieval Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class SecureBatchRetrievalProtocol: public SecureRetrievalProtocol<DataType> {
private:
    int _number_of_batches;

public:
    SecureBatchRetrievalProtocol(SecureBatchRetrievalServer<DataType>& server, SecureBatchRetrievalClient<DataType>& client):
            SecureRetrievalProtocol<DataType>(server, client) {}

    // TODO: Consider to delete
    vector<int> retrieve_batch(int batch_index) {
//        TODO: fill
        vector<int> temp;
        return temp;
    }

    // TODO: Export to tester class
    void retrieve_all_matches() {
        for(auto batch_index: range(_number_of_batches)) {
            retrieve_batch(batch_index);
        }
    }
};

/**
 * This class implement Secure Report Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class SecureReportProtocol: public SecureRetrievalProtocol<DataType> {
public:
    SecureReportProtocol(SecureReportServer<DataType>& server, SecureReportClient<DataType>& client):
            SecureRetrievalProtocol<DataType>(server, client) {}
};

#endif //SECURE_PROTOCOL_H
