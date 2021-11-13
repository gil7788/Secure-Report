//
// Created by gil on 20.11.2020.
//

#ifndef SECURE_PROTOCOL_H
#define SECURE_PROTOCOL_H

#include <iostream>
#include <memory>
#include "../infrastructure/VectorUtils.h"
#include "Client.h"
#include "Server.h"


using namespace std;

// TODO:
//  1. Add Aproximate counting
//  2. Add Slackness

/**
 * This class implement virtual Secure Retrieval Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */

/**
     * Initialize protocol's entities: client, server and trusted third party
     */
template <typename DataType>
class SecureRetrievalProtocol {
private:
    unique_ptr<TrustedThirdParty> _public_server{new TrustedThirdParty};

public:
    /**
     * Initialize protocol's entities: client, server and trusted third party
     */
    void initialize() {
        initialize_client();
        initialize_server();
    }
    /**
     * Client encrypt plain database data and upload encrypted data to the server.
     * @param plain_data
     * @return Upload successfully
     */
    bool upload(vector<int>& plain_data) {
        auto encrypted_data = get_client()->upload_data_to_server(plain_data);
        bool uploaded_successful = get_server()->upload(encrypted_data);
        return uploaded_successful;
    }

    /**
     * Client encrypt PlainQuery and send EncryptedQuery to Server, to proccess it and evaluate desired encrypted query.
     * To query Server, first Client should upload data to Server, by SecureRetrievalProtocol::upload.
     * @param query Client selected PlainQuery
     */
    void query(unique_ptr<PlainQuery<DataType>> query) {
        get_client()->initialize_query(move(query));
        auto encrypted_query = get_client()->encrypt_query();
//        TODO pass query refernce
        get_server()->initialize_query(*encrypted_query);
    }

    /**
     * Client retrieve output from server, evaluated based on EncryptedQuery (see SecureRetrievalProtocol::query).
     * Client evaluate plain desired result by decrypt and decode output from Server.
     * @return Matches indices
     */
    virtual std::vector<int> retrieve() = 0;

protected:
    void initialize_client() {
        get_client()->initialize();
    }

    void initialize_server() {
        auto public_server = get_server()->initialize();
        _public_server = move(public_server);
    }

    virtual unique_ptr<Server<DataType>> const& get_server() = 0;

    virtual unique_ptr<Client<DataType>> const& get_client() = 0;

    unique_ptr<TrustedThirdParty>& get_trusted_third_party() {
        return _public_server;
    }
};

/**
 * This class implement Secure Batch Retrieval Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename DataType>
class SecureBatchRetrievalProtocol: public SecureRetrievalProtocol<DataType> {
private:
    unique_ptr<Server<DataType>> _server;
    unique_ptr<Client<DataType>> _client;
    unique_ptr<EncryptedQuery<DataType>> _encrypted_query;

public:
    SecureBatchRetrievalProtocol(unique_ptr<SecureBatchRetrievalServer<DataType>> server,
                                 unique_ptr<SecureBatchRetrievalClient<DataType>> client):
                                _server{move(server)}, _client{move(client)}, SecureRetrievalProtocol<DataType>() {}

   /**
   * Client encrypt PlainQuery and send EncryptedQuery to Server, to proccess it and evaluate desired encrypted query.
   * To query Server, first Client should upload data to Server, by SecureRetrievalProtocol::upload.
   * @param query Client selected PlainQuery
   */
   std::vector<int> retrieve() {
       std::vector<int> matches;
       auto number_of_batches = get_query().get_number_of_batches();
       for (int i = 0; i < number_of_batches; ++i) {
           auto batch = retrieve_batch();
           matches.insert( matches.end(), batch.begin(), batch.end() );
       }
       return matches;
   }

    vector<int> retrieve_batch() {
        const auto& public_server = SecureRetrievalProtocol<DataType>::get_trusted_third_party();

        std::vector<DataType> encrypted_matches_indices = get_server()->send_matches_indices_to_client(public_server);
        std::vector<int> matches_indices = get_client()->retrieve_matches_indices(encrypted_matches_indices, public_server);
        return matches_indices;
    }

private:
    unique_ptr<Server<DataType>> const& get_server() override {
        return _server;
    }

    unique_ptr<Client<DataType>> const& get_client() override {
        return _client;
    }

    EncryptedMultipleBatchRetrievalQuery<DataType> get_query() {
//       TODO Add pointer deletion
        SecureBatchRetrievalServer<DataType> *sbr_server = dynamic_cast<SecureBatchRetrievalServer<DataType>*>(get_server().get());
        return sbr_server->_query;
    }
};

/**
 * This class implement Secure Report Protocol API.
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename DataType>
class SecureReportProtocol: public SecureRetrievalProtocol<DataType> {
public:
    unique_ptr<Server<DataType>> _server;
    unique_ptr<Client<DataType>> _client;
    unique_ptr<EncryptedQuery<DataType>> _encrypted_query;

    SecureReportProtocol(unique_ptr<SecureReportServer<DataType>> server, unique_ptr<SecureReportClient<DataType>> client) :
            _client{move(client)}, _server{move(server)} {}

    /**
     * Client retrieve output from server, evaluated based on EncryptedQuery (see SecureRetrievalProtocol::query).
     * Client evaluate plain desired result by decrypt and decode output from Server.
     * @return Matches indices
     */
    std::vector<int> retrieve() override {
        const auto& public_server = SecureRetrievalProtocol<DataType>::get_trusted_third_party();
        std::vector<DataType> encrypted_matches_indices = get_server()->send_matches_indices_to_client(public_server);

        std::vector<int> matches_indices = get_client()->retrieve_matches_indices(encrypted_matches_indices, public_server);
        return matches_indices;
    }

private:
    unique_ptr<Server<DataType>> const& get_server() override {
        return _server;
    }

    unique_ptr<Client<DataType>> const& get_client() override {
        return _client;
    }

    unique_ptr<EncryptedQuery<DataType>> const& get_query() {
        return move(_encrypted_query);
    }
};

#endif //SECURE_PROTOCOL_H
