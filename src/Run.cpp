//
// Created by gil on 28/04/20.
//

#include <iostream>

#include "algorithms/fully_homomorphic/GenericZP.h"
#include "algorithms/fully_homomorphic/SimplifiedHelibNumber.h"
#include "entities/Protocol.h"
#include "infrastructure/test/test_case/SecureRetrievalTestCase.h"
#include "infrastructure/test/tester/ProtocolTester.h"


#define SIMD_FACTOR constants::WORD_LENGTH
using namespace std;

// TODO implement protocol_tester class
PlainContext get_plain_data_type() {
    long r = constants::WORD_LENGTH;
    PlainContext generic_plain_data_type = PlainContext(r);
    generic_plain_data_type.initialize();
    return generic_plain_data_type;
}

EncryptedContext get_encrypted_data_type(int size) {
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

    const std::string key_file_path = constants::KEY_FILE_PATH;
    EncryptedContext encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);
    return encrypted_data_type;
}

unique_ptr<TestCase> test_plain_secure_report_protocol(int lookup_value,
                                                int database_size,
                                                vector<int> matches_indices,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    const string test_name = "Plain Secure Report Retrieval";
    PlainContext generic_plain_data_type = get_plain_data_type();
    unique_ptr<VirtualContext> data_type_ptr(new PlainContext(generic_plain_data_type));

    unique_ptr<TestCase> test_case_ptr(new SecureReportTestCase<GenericZP>(test_name, lookup_value, database_size, matches_indices,
                                 isMatch, plain_data, data_type_ptr));
                
    return test_case_ptr;
}

unique_ptr<TestCase> test_encrypted_secure_report_protocol(int lookup_value,
                                                    int database_size,
                                                    vector<int> matches_indices,
                                                    SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                    std::vector<int>& plain_data) {
    const string test_name = "Encrypted Secure Report Retrieval";
    unique_ptr<VirtualContext> data_type_ptr = make_unique<EncryptedContext>(get_encrypted_data_type(database_size));

    unique_ptr<TestCase> test_case_ptr = make_unique<SecureReportTestCase<SimplifiedHelibNumber>>(test_name, lookup_value, database_size, matches_indices,
                                 isMatch, plain_data, data_type_ptr);

    return test_case_ptr;
}

unique_ptr<TestCase> test_plain_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int database_size,
                                                vector<int> matches_indices,
                                                int requested_number_of_matches,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    const string test_name = "Plain Secure Batch Retrieval";
    PlainContext generic_plain_data_type = get_plain_data_type();
    unique_ptr<VirtualContext> data_type_ptr(new PlainContext(generic_plain_data_type));

    unique_ptr<TestCase> test_case_ptr(new SecureBatchRetrievalTestCase<GenericZP>(test_name, lookup_value, database_size, requested_number_of_matches, matches_indices,
                                 isMatch, plain_data, data_type_ptr, batch_size));

    return test_case_ptr;
}

unique_ptr<TestCase> test_encrypted_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int database_size,
                                                vector<int> matches_indices,
                                                int requested_number_of_matches,
                                                SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                std::vector<int>& plain_data) {

    string test_name = "Encrypted Secure Batch Retrieval";
    unique_ptr<VirtualContext> data_type_ptr(new EncryptedContext(get_encrypted_data_type(database_size)));

    unique_ptr<TestCase> test_case_ptr(new SecureBatchRetrievalTestCase<SimplifiedHelibNumber>(test_name, lookup_value, database_size, requested_number_of_matches, matches_indices,
                                    isMatch, plain_data, data_type_ptr, batch_size));
    
    return test_case_ptr;
}

std::vector<std::unique_ptr<TestCase>> generate_plain_secure_report_tests(int min_database_size, int max_database_size, int lookup_value,
                                                int number_of_matches_in_database,
                                                GenericZP (*is_match)(GenericZP&, GenericZP&)) {
    std::vector<std::unique_ptr<TestCase>> test_cases;
    int tests = 15;
    int size_delta = max_database_size - min_database_size;
    int step_size = size_delta/tests;
    vector<int> database_sizes;
    
    for(int i = 0; i < tests; ++i) {
        int database_size = min_database_size + int(ceil(i* step_size));
        database_sizes.push_back(database_size);
    }

    for(auto& database_size: database_sizes) {
        cout << "Database size: "  << database_size << "\n";
        std::vector<int> data = VectorUtils::generate_binary_std_vector(database_size, number_of_matches_in_database);
        // TODO: consider to delete
        vector<int> matches_indices = VectorUtils::get_matches_std_vector(data, lookup_value);
        string matches_indices_string = VectorUtils::std_vector_to_string(matches_indices);
        std::cout << "Selected indices: " << matches_indices_string << std::endl;
        test_cases.push_back(move(test_plain_secure_report_protocol(lookup_value, database_size, matches_indices, is_match, data)));
    }
    
    return test_cases;
}

std::vector<std::unique_ptr<TestCase>> generate_plain_secure_batch_retrieval_tests(int min_database_size,
                                                                                   int max_database_size,
                                                                                   int lookup_value,
                                                                                   int number_of_matches_in_database,
                                                                                   GenericZP (*is_match)(GenericZP&, GenericZP&),
                                                                                   int batch_size,
                                                                                   int requested_number_of_matches) {
    std::vector<std::unique_ptr<TestCase>> test_cases;
    int tests = 15;
    int size_delta = max_database_size - min_database_size;
    int step_size = size_delta/tests;
    vector<int> database_sizes;

    for(int i = 0; i < tests; ++i) {
        int database_size = min_database_size + int(ceil(i* step_size));
        database_sizes.push_back(database_size);
    }


    for(auto& database_size: database_sizes) {
        cout << "Database size: "  << database_size << "\n";
        std::vector<int> data = VectorUtils::generate_binary_std_vector(database_size, number_of_matches_in_database);
        // TODO: consider to delete
        vector<int> matches_indices = VectorUtils::get_matches_std_vector(data, lookup_value);
        string matches_indices_string = VectorUtils::std_vector_to_string(matches_indices);
        std::cout << "Selected indices: " << matches_indices_string << std::endl;

        test_cases.push_back(move(
                test_plain_secure_batch_retrieval_protocol(
                        lookup_value, batch_size, database_size, matches_indices, requested_number_of_matches, is_match, data)));
    }

    return test_cases;
}


int main(int argc, char** argv) {
    std::cout << "Current path is: " << fs::current_path() << "\n";
    int size = 2048;
    int number_of_matches_in_database = 4;
    int batch_size = 2;
    int number_of_requested_matches = batch_size*4;
    int batch_size_number_of_matches = 2;
    int all_matches = 4;

    int r = constants::WORD_LENGTH;

    int one_lookup_value = 1;
    int zero_lookup_value = 0;
    int arbitrary_lookup_value = 2;
    auto binary_arbitrary_lookup_value = VectorUtils::number_to_std_vector(arbitrary_lookup_value, (int) r);

    auto genericis_match =  Comparators<GenericZP>::areEqualBinary;
    auto generic_lessThan = Comparators<GenericZP>::lessThan;
    auto encrypted_lessThan = Comparators<SimplifiedHelibNumber>::lessThan;
    auto encryptedis_match =  Comparators<SimplifiedHelibNumber>::areEqualBinary;

    std::vector<int> ones_data = VectorUtils::generate_binary_std_vector(size, number_of_matches_in_database);
    std::vector<int> zeros_data = VectorUtils::generate_binary_std_vector(size, size-number_of_matches_in_database);
    std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(size, number_of_matches_in_database, arbitrary_lookup_value);

    int number_of_batches = ceil( ((double) number_of_matches_in_database / batch_size));

    int min_database_size = 1024;
    vector<int> number_of_matches_in_databases{4, 8, 16};
    int max_database_size = min_database_size * 64;
//    for(auto& matches: number_of_matches_in_databases) {
//        std::vector<std::unique_ptr<TestCase>> test_cases = generate_plain_secure_report_tests(min_database_size, max_database_size ,one_lookup_value, number_of_matches_in_database, genericis_match);
//        string log_name = "run_test_matches-" + to_string(matches);
//        ProtocolTester tester(log_name);
//        tester.initialize(test_cases);
//        tester.test_and_log_all();
//    }

    for(auto& matches: number_of_matches_in_databases) {
        auto test_cases =
                generate_plain_secure_batch_retrieval_tests(
                        min_database_size, max_database_size, one_lookup_value, number_of_matches_in_database,
                        genericis_match, batch_size, number_of_requested_matches);

        string log_name = "run_test_matches-" + to_string(matches);
        ProtocolTester tester(log_name);
        tester.initialize(test_cases);
        tester.test_and_log_all();
    }

//    std::vector<std::unique_ptr<TestCase>> test_cases = generate_plain_secure_report_tests(min_database_size, min_database_size ,one_lookup_value, 4, genericis_match);
//    string log_name = "run_test_matches-" + to_string(4);
//    ProtocolTester tester(log_name);
//    tester.initialize(test_cases);
//    tester.test_and_log_all();
    

    // test_cases.push_back(move(test_plain_secure_report_protocol(one_lookup_value, size, number_of_matches_in_database, genericis_match, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_report_protocol(one_lookup_value, size, number_of_matches_in_database, encryptedis_match, ones_data)));
    // test_cases.push_back(move(test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, batch_size_number_of_matches, size, number_of_matches_in_database, genericis_match, ones_data)));
    // test_cases.push_back(move(test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, all_matches, size, number_of_matches_in_database, genericis_match, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, batch_size_number_of_matches_in_database, size, number_of_matches_in_database, encryptedis_match, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, all_matches, size, number_of_matches_in_database, encryptedis_match, ones_data)));

    
}

// TODO:
// 1. Finish implement ProtocolTestCase class for SBR - retrieval by batches
// 2. Implement In Protocol.cpp, SBR protocol, functionality to 1. retrieve batch, 2. retrieve all batches, 3. get number of batches 
// 3. Test cases builder class