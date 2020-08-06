//
// Created by gil on 28/04/20.
//

#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEDatabase.h"
#include "Server.h"

#include "FileUtils.h"

#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

template <typename DataType>
bool upload(Client<DataType>& client, Server<DataType>& server, std::vector<int> plain_data, int comparison_value) {
    std::vector<int> indices = VectorUtils::get_matches_std_vector(plain_data, comparison_value);

    string s = VectorUtils::std_vector_to_string(indices);
    std::cout << "Selected indices: " << s << std::endl;

    // Client
    client.initialize();
    auto encrypted_data = client.upload_data_to_server(plain_data);

    // Server
    server.initialize();
    bool uploaded_successful = server.upload(encrypted_data);
    return uploaded_successful;
}

template <typename DataType>
std::vector<int> query(Client<DataType>& client, Server<DataType>& server, TrustedThirdParty& trusted_third_party,
           int lookup_value, DataType (*isMatch)(DataType&, DataType&, int)) {
    // Client
    SecureReportQuery<DataType> query(lookup_value, isMatch);
    auto encrypted_query = client.query_server(query);

    // Server
    std::vector<DataType> encrypted_matches_indices = server.send_matches_indices_to_client(encrypted_query, trusted_third_party);

    //Client
    std::vector<int> matches_indices = client.retrieve_matches_indices(encrypted_matches_indices, trusted_third_party);
    return matches_indices;
}

template <typename DataType>
void test_client(Client<DataType>& client, Server<DataType>& server, TrustedThirdParty& trusted_third_party,
        int lookup_value, DataType (*isMatch)(DataType&, DataType&, int), std::vector<int> plain_data) {
    bool uploaded_successful = upload<DataType>(client, server, plain_data, lookup_value);

    if(! uploaded_successful) {
        std::cout << "Failed to upload data\n";
        return;
    }

    std::vector<int> matches_indices = query(client, server, trusted_third_party, lookup_value, isMatch);

    string result_string = VectorUtils::std_vector_to_string(matches_indices);
    std::cout << "Result: " << result_string << std::endl;
}

void test_plain_client(int database_size, int sparsity, PlainDataType plain_data_type,
                       int lookup_value, MyZP (*isMatch)(MyZP &, MyZP &, int), std::vector<int> plain_data) {

    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<MyZP> client(database_size, sparsity, plain_data_type);
    auto database = PlainServer(database_size, sparsity, plain_data_type);
    test_client(client, database, trusted_third_party, lookup_value, isMatch, plain_data);
}

void test_encrypted_client(int database_size, int sparsity, EncryptedDataTypeFromParameters encrypted_data_type,
                           int lookup_value, HelibNumber (*isMatch)(HelibNumber &, HelibNumber &, int), std::vector<int> plain_data) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<HelibNumber> client(database_size, sparsity, encrypted_data_type);
    auto database = EncryptedServer(database_size, sparsity, encrypted_data_type);
    test_client(client, database, trusted_third_party, lookup_value, isMatch, plain_data);
}

int main(int argc, char** argv) {
    int size = 2048;
    int sparsity = 3;

    long r = 1;
    int L = 7;
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

    int one_lookup_value = 1;
    int zero_lookup_value = 0;
    int arbitrary_lookup_value = 48;
    auto plain_isMatch =  FHEUtils<MyZP>::areEqualBinary;
    auto encrypted_isMatch =  FHEUtils<HelibNumber>::areEqualBinary;

    std::vector<int> ones_data = VectorUtils::generate_binary_std_vector(size, sparsity);
    std::vector<int> zeros_data = VectorUtils::generate_binary_std_vector(size, size-sparsity);
    std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(size, sparsity, arbitrary_lookup_value);

    PlainDataType plain_data_type = PlainDataType();
    EncryptedDataTypeFromParameters encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);

    test_plain_client(size, sparsity, plain_data_type, one_lookup_value, plain_isMatch, ones_data);
    test_plain_client(size, sparsity, plain_data_type, zero_lookup_value, plain_isMatch, zeros_data);
//    test_plain_client(size, sparsity, plain_data_type, arbitrary_lookup_value, plain_isMatch, arbitrary_data);
    test_encrypted_client(size, sparsity, encrypted_data_type, one_lookup_value, encrypted_isMatch, ones_data);
    test_encrypted_client(size, sparsity, encrypted_data_type, zero_lookup_value, encrypted_isMatch, zeros_data);
}
