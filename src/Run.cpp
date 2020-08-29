//
// Created by gil on 28/04/20.
//

#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEDatabase.h"
#include "Server.h"
#include "FileUtils.h"
#include "GenericZP.h"
#include <zp.h>
#include "SimplifiedHelibNumber.h"
#include "Queries.h"

#include <random>
#include <iostream>

#define SIMD_FACTOR constants::WORD_LENGTH
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
template <typename DataType>
bool upload(Client<DataType>& client, Server<DataType>& server, std::vector<int> plain_data, int comparison_value) {
    std::vector<int> indices = VectorUtils::get_matches_std_vector(plain_data, comparison_value);

    string s = VectorUtils::std_vector_to_string(indices);
    std::cout << "Selected indices: " << s << std::endl;

    // Client
    auto encrypted_data = client.upload_data_to_server(plain_data);

    // Server
    bool uploaded_successful = server.upload(encrypted_data);
    return uploaded_successful;
}

template <typename DataType>
TrustedThirdParty initialize(Server<DataType>& server) {
    // Public server
    TrustedThirdParty public_server = server.construct_public_server();
    public_server.initialize();
    return public_server;
}

template <typename DataType>
std::vector<int> retrieve(Client<DataType>& client, Server<DataType>& server, TrustedThirdParty& public_server) {
    // Server
    std::vector<DataType> encrypted_matches_indices = server.send_matches_indices_to_client(public_server);

    //Client
    std::vector<int> matches_indices = client.retrieve_matches_indices(encrypted_matches_indices, public_server);
    return matches_indices;
}

GenericPlainDataType get_plain_data_type() {
    long r = constants::WORD_LENGTH;
    GenericPlainDataType generic_plain_data_type = GenericPlainDataType(r);
    generic_plain_data_type.initialize();
    return generic_plain_data_type;
}

EncryptedDataTypeFromParameters get_encrypted_data_type(int size) {
    long r = constants::WORD_LENGTH;
    int L = (int) ceil(log2(r)) + 2;
    int p = 2;

    long R = 1;
    long d = 1;
    long c = 2;
    long k = 80;
    long s = 0;
    long chosen_m = 0;
    Vec<long> gens;
    Vec<long> ords;
    const std::string key_file_path = "../../key";
    EncryptedDataTypeFromParameters encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);
    encrypted_data_type.initialize();
    return encrypted_data_type;
}

template <typename DataType>
void test_secure_report(SecureReportClient<DataType>& client,
                        SecureReportServer<DataType>& server,
                                        int lookup_value,
                                        int database_size,
                                        int number_of_matches,
                                        DataType (*isMatch)(DataType&, DataType&),
                                        std::vector<int>& plain_data) {
    bool uploaded_successful = upload<DataType>(client, server, plain_data, lookup_value);

    if(! uploaded_successful) {
        std::cout << "Failed to upload data\n";
        return;
    }

    TrustedThirdParty public_server = initialize<DataType>(server);

    client.initialize_query(lookup_value, isMatch);
    auto encrypted_query = client.encrypt_query();
    server.initialize_query(encrypted_query);
    std::vector<int> matches_indices =
            retrieve<DataType>(client, server, public_server);

    string result_string = VectorUtils::std_vector_to_string(matches_indices);
    std::cout << "Result: " << result_string << std::endl;
}

void test_encrypted_secure_report(int lookup_value,
                                           int database_size,
                                           int number_of_matches,
                                           SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                           std::vector<int>& plain_data) {

    EncryptedDataTypeFromParameters encrypted_plain_data_type = get_encrypted_data_type(database_size);
    SecureReportClient<SimplifiedHelibNumber> client(database_size, number_of_matches, encrypted_plain_data_type);
    SecureReportServer<SimplifiedHelibNumber> server(database_size, number_of_matches, encrypted_plain_data_type);

    server.initialize();

    test_secure_report<SimplifiedHelibNumber>(client, server, lookup_value,
                                                       database_size, number_of_matches, isMatch, plain_data);
}

void test_plain_secure_report(int lookup_value,
                                       int database_size,
                                       int number_of_matches,
                                       GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                       std::vector<int>& plain_data) {

    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    SecureReportClient<GenericZP> client(database_size, number_of_matches, generic_plain_data_type);
    SecureReportServer<GenericZP> server(database_size, number_of_matches, generic_plain_data_type);

    server.initialize();

    test_secure_report<GenericZP>(client, server, lookup_value,
                                           database_size, number_of_matches, isMatch, plain_data);
}

// =========== Secure Batch Retrieval =============
template<typename DataType>
void test_secure_batch_retrieval(SecureBatchRetrievalClient<DataType>& client,
                                SecureBatchRetrievalServer<DataType>& server,
                                int lookup_value,
                                 int batch_size,
                                 int batch_index,
                                 int database_size,
                                 int number_of_matches,
                                 DataType (*isMatch)(DataType&, DataType&),
                                 std::vector<int>& plain_data) {

    bool uploaded_successful = upload<DataType>(client, server, plain_data, lookup_value);

    if(! uploaded_successful) {
        std::cout << "Failed to upload data\n";
        return;
    }

    TrustedThirdParty public_server = initialize<DataType>(server);

    client.initialize_query(lookup_value, batch_size, batch_index, database_size, number_of_matches, isMatch);
    auto encrypted_query = client.encrypt_query();
    server.initialize_query(encrypted_query);
    // Client
    std::vector<int> matches_indices =
            retrieve<DataType>(client, server, public_server);

    string result_string = VectorUtils::std_vector_to_string(matches_indices);
    std::cout << "Result: " << result_string << std::endl;
}

void test_encrypted_secure_batch_retrieval(int lookup_value,
                                           int batch_size,
                                           int batch_index,
                                           int database_size,
                                           int number_of_matches,
                                           SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                           std::vector<int>& plain_data) {

    EncryptedDataTypeFromParameters encrypted_plain_data_type = get_encrypted_data_type(database_size);
    SecureBatchRetrievalClient<SimplifiedHelibNumber> client(database_size, number_of_matches, encrypted_plain_data_type);
    SecureBatchRetrievalServer<SimplifiedHelibNumber> server(database_size, number_of_matches, encrypted_plain_data_type);

    server.initialize();

    test_secure_batch_retrieval<SimplifiedHelibNumber>(client, server, lookup_value, batch_size, batch_index,
            database_size, number_of_matches, isMatch, plain_data);
}

void test_plain_secure_batch_retrieval(int lookup_value,
                                       int batch_size,
                                       int batch_index,
                                       int database_size,
                                       int number_of_matches,
                                       GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                       std::vector<int>& plain_data) {

    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    SecureBatchRetrievalClient<GenericZP> client(database_size, number_of_matches, generic_plain_data_type);
    SecureBatchRetrievalServer<GenericZP> server(database_size, number_of_matches, generic_plain_data_type);
    client.initialize();
    server.initialize();

    test_secure_batch_retrieval<GenericZP>(client, server, lookup_value, batch_size, batch_index,
                                                       database_size, number_of_matches, isMatch, plain_data);
}

int main(int argc, char** argv) {
    std::cout << "Current path is: " << fs::current_path() << "\n";
    int size = 128;
    int number_of_matches = 3;
    int batch_size = 4;

    int r = constants::WORD_LENGTH;

    int one_lookup_value = 1;
    int zero_lookup_value = 0;
    int arbitrary_lookup_value = 2;
    auto binary_arbitrary_lookup_value = VectorUtils::number_to_std_vector(arbitrary_lookup_value, (int) r);

    auto generic_isMatch =  FHEUtils<GenericZP>::areEqualBinary;
    auto generic_lessThan = FHEUtils<GenericZP>::lessThan;
    auto encrypted_lessThan = FHEUtils<SimplifiedHelibNumber>::lessThan;
    auto encrypted_isMatch =  FHEUtils<SimplifiedHelibNumber>::areEqualBinary;

    std::vector<int> ones_data = VectorUtils::generate_binary_std_vector(size, number_of_matches);
    std::vector<int> zeros_data = VectorUtils::generate_binary_std_vector(size, size-number_of_matches);
    std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(size, number_of_matches, arbitrary_lookup_value);


//    serial_test(size, number_of_matches, encrypted_data_type, 100, encrypted_isMatch);
//    int number_of_batches = ceil( ((double) number_of_matches / batch_size));
//    for (int i = 0; i < number_of_batches; ++i) {
//        cout << "Plain equals SBR - batch " << i << ": \n";
//        test_plain_secure_batch_retrieval(one_lookup_value, batch_size, i, size, number_of_matches, generic_isMatch, ones_data);
//    }
//
//    for (int i = 0; i < number_of_batches; ++i) {
//        cout << "Encrypted equals SBR - batch " << i << ": \n";
//        test_encrypted_secure_batch_retrieval(one_lookup_value, batch_size, i, size, number_of_matches, encrypted_isMatch, ones_data);
//    }


    cout << "Plain equals SR \n";
    test_plain_secure_report(one_lookup_value, size, number_of_matches, generic_isMatch, ones_data);
    cout << "Encrypted equals SR \n";
    test_encrypted_secure_report(one_lookup_value, size, number_of_matches, encrypted_isMatch, ones_data);     int)>(encrypted_lessThan), arbitrary_data);
}
