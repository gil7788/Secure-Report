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
bool test_database(Database<DataType>& database) {
    VectorXi plain_data = VectorUtils::generate_binary_vector(database._size, database._sparsity);
    std::vector<int> indices = VectorUtils::getMatches(plain_data);

    std::unique_ptr<DataType> lookup_value = std::unique_ptr<DataType>(new DataType(1));

    // connect db
    database.upload(plain_data);
    std::unique_ptr<DataType[]> encrypted_matches = database.evaluate_matches_indices(lookup_value, FHEUtils<DataType>::areEqualBinary);
    VectorXi matches = database.client_retrieve_matches_indices(encrypted_matches);

    database._io.output("matches size: " + std::to_string(matches.size()) +"\n", constants::OUTPUT_LEVELS::DEBUG);
    for (auto i = 0; i < matches.size(); ++i)
        database._io.output(std::to_string(matches(i)) + " ", constants::OUTPUT_LEVELS::DEBUG);

    // Print input and result
    database._io.output("Input indices: \n", constants::OUTPUT_LEVELS::DEBUG);
    for (auto i = indices.begin(); i != indices.end(); ++i)
        std::cout << *i << ' ';
    database._io.output("\n Decoded matches: \n", constants::OUTPUT_LEVELS::DEBUG);

    // Test if result is valid
    std::vector<int> decoded_std(matches.data(), matches.data() + matches.rows() * matches.cols());
    bool is_successful_decode = std::is_permutation(indices.begin(), indices.end(), decoded_std.begin());

    return is_successful_decode;
}

void test_plain_database(RegularDatabase& database) {
    bool is_successful = test_database<MyZP>(database);
    std::cout << "Plain Success: " << is_successful << std::endl;
}

void test_encrypted_database(EncryptedDatabase& database) {
    bool is_successful = test_database<HelibNumber>(database);
    std::cout << "Encrypted Success: " << is_successful << std::endl;
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
    RegularDatabase plain_database(size, sparsity, plain_data_type);
    plain_database.initialize_data_type();
    test_plain_database(plain_database);


    EncryptedDataType encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords);
    EncryptedDatabase encrypted_database(size, sparsity, encrypted_data_type);
    encrypted_database.initialize_data_type();
    test_encrypted_database(encrypted_database);
}
