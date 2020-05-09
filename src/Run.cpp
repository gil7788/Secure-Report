//
// Created by gil on 28/04/20.
//

#include <iostream>

#include <zp.h>
#include <eq.h>

#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"
#include "Database.h"


#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

template <typename DataType>
void test_client(Client<DataType>& client, Database<DataType>& server, TrustedThirdParty& trusted_third_party) {
    // Client
    client.initialize();

    std::vector<int> plain_data =
            VectorUtils::generate_binary_std_vector(client._datase_size, client._maximal_number_of_matches_per_query);
    std::vector<int> indices = VectorUtils::get_matches_std_vector(plain_data);

    auto encrypted_data = client.upload_data_to_server(plain_data);

    // Server
    server.initialize_data_type();
    server.upload(plain_data);

    // Client
    int lookup_value = 1;
    SecureReportQuery<DataType> query(lookup_value, FHEUtils<DataType>::areEqualBinary);
    auto encrypted_query = client.query_server(query);

    // Server
    std::vector<DataType> matches_indices = server.send_matches_indices_to_client(encrypted_query, trusted_third_party);

    //Client
    auto result = client.retrieve_matches_indices(matches_indices, trusted_third_party);

    string s = VectorUtils::std_vector_to_string(indices);
    string result_string = VectorUtils::std_vector_to_string(result);


    std::cout << "Selected indices: " << s << std::endl;

    std::cout << "Result: " << result_string << std::endl;

}

void test_plain_client(int database_size, int sparsity, PlainDataType plain_data_type) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<MyZP> client(database_size, sparsity, plain_data_type);
    auto database = RegularDatabase(database_size, sparsity, plain_data_type);
    test_client(client, database, trusted_third_party);
}

void test_encrypted_client(int database_size, int sparsity, EncryptedDataType encrypted_data_type) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<HelibNumber> client(database_size, sparsity, encrypted_data_type);
    auto database = EncryptedDatabase(database_size, sparsity, encrypted_data_type);
    test_client(client, database, trusted_third_party);
}

int main(int argc, char** argv) {
    int size = 128;
    int sparsity = 3;

    long r = 1;
    int L = 7;
    int p = Primes::find_prime_bigger_than(size);

    long R = 1;
    long d = 1;
    long c = 2;
    long k = 80;
    long s = 0;
    long chosen_m = 0;
    Vec<long> gens;
    Vec<long> ords;

    PlainDataType plain_data_type(sparsity);
    EncryptedDataType encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords);

    test_plain_client(size, sparsity, plain_data_type);
    test_encrypted_client(size, sparsity, encrypted_data_type);
}
