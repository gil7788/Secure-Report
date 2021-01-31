//
// Created by gil on 28/04/20.
//

#include <iostream>

#include "GenericZP.h"
#include "SimplifiedHelibNumber.h"
#include "Protocol.h"
#include "utils/test/test_case/SecureRetrievalTestCase.h"
#include "utils/test/tester/ProtocolTester.h"


#define SIMD_FACTOR constants::WORD_LENGTH
using namespace std;

// TODO implement protocol_tester class
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

    const std::string key_file_path = constants::KEY_FILE_PATH;
    EncryptedDataTypeFromParameters encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);
    return encrypted_data_type;
}

unique_ptr<TestCase> test_plain_secure_report_protocol(int lookup_value,
                                                int database_size,
                                                int number_of_matches_in_database,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    const string test_name = "Plain Secure Report Retrieval";
    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    unique_ptr<DatabaseDataType> data_type_ptr(new GenericPlainDataType(generic_plain_data_type));

    unique_ptr<TestCase> test_case_ptr(new SecureReportTestCase<GenericZP>(test_name, lookup_value, database_size, number_of_matches_in_database,
                                 isMatch, plain_data, data_type_ptr));

    SecureReportTestCase<GenericZP> test_case(test_name, lookup_value, database_size, number_of_matches_in_database,
                                 isMatch, plain_data, data_type_ptr);

                
    return test_case_ptr;
}

unique_ptr<TestCase> test_encrypted_secure_report_protocol(int lookup_value,
                                                    int database_size,
                                                    int number_of_matches_in_database,
                                                    SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                    std::vector<int>& plain_data) {
    const string test_name = "Encrypted Secure Report Retrieval";
    unique_ptr<DatabaseDataType> data_type_ptr = make_unique<EncryptedDataTypeFromParameters>(get_encrypted_data_type(database_size));

    unique_ptr<TestCase> test_case_ptr = make_unique<SecureReportTestCase<SimplifiedHelibNumber>>(test_name, lookup_value, database_size, number_of_matches_in_database,
                                 isMatch, plain_data, data_type_ptr);

    return test_case_ptr;
}

unique_ptr<TestCase> test_plain_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int number_of_matches_in_database,
                                                int requested_number_of_matches,
                                                int database_size,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    const string test_name = "Plain Secure Batch Retrieval";
    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    unique_ptr<DatabaseDataType> data_type_ptr(new GenericPlainDataType(generic_plain_data_type));

    unique_ptr<TestCase> test_case_ptr(new SecureBatchRetrievalTestCase<GenericZP>(test_name, lookup_value, database_size, number_of_matches_in_database, requested_number_of_matches,
                                 isMatch, plain_data, data_type_ptr, batch_size));

    return test_case_ptr;
}

unique_ptr<TestCase> test_encrypted_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int number_of_matches_in_database,
                                                int database_size,
                                                int requested_number_of_matches,
                                                SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                std::vector<int>& plain_data) {

    string test_name = "Encrypted Secure Batch Retrieval";
    unique_ptr<DatabaseDataType> data_type_ptr(new EncryptedDataTypeFromParameters(get_encrypted_data_type(database_size)));

    unique_ptr<TestCase> test_case_ptr(new SecureBatchRetrievalTestCase<SimplifiedHelibNumber>(test_name, lookup_value, database_size, number_of_matches_in_database, requested_number_of_matches,
                                    isMatch, plain_data, data_type_ptr, batch_size));
    
    return test_case_ptr;
}

std::vector<std::unique_ptr<TestCase>> generate_plain_secure_report_tests(int min_database_size, int max_database_size, int lookup_value,
                                                int number_of_matches_in_database,
                                                GenericZP (*is_match)(GenericZP&, GenericZP&)) {
    std::vector<std::unique_ptr<TestCase>> test_cases;
    int tests = 25;
    int size_delta = max_database_size - min_database_size;
    int step_size = size_delta/tests;
    vector<int> database_sizes;
    
    for(int i = 0; i < tests; ++i) {
        int database_size = min_database_size + int(ceil(i* step_size));
        database_sizes.push_back(database_size);
    }

    // int tests = 10;
    // for(int i = 0; i < tests; ++i) {
    //     int database_size = min_database_size * int(pow(2, i));
    //     database_sizes.push_back(database_size);
    // }

    for(auto& database_size: database_sizes) {
        cout << "Database size: "  << database_size << "\n";
        std::vector<int> data = VectorUtils::generate_binary_std_vector(database_size, number_of_matches_in_database);
        test_cases.push_back(move(test_plain_secure_report_protocol(lookup_value, database_size, number_of_matches_in_database, is_match, data)));
    }
    
    return test_cases;
}


int main(int argc, char** argv) {
    std::cout << "Current path is: " << fs::current_path() << "\n";
    int size = 2048;
    int number_of_matches_in_database = 8;
    int batch_size = 2;
    int batch_size_number_of_matches = 2;
    int all_matches = 4;

    int r = constants::WORD_LENGTH;

    int one_lookup_value = 1;
    int zero_lookup_value = 0;
    int arbitrary_lookup_value = 2;
    auto binary_arbitrary_lookup_value = VectorUtils::number_to_std_vector(arbitrary_lookup_value, (int) r);

    auto generic_isMatch =  FHEUtils<GenericZP>::areEqualBinary;
    auto generic_lessThan = FHEUtils<GenericZP>::lessThan;
    auto encrypted_lessThan = FHEUtils<SimplifiedHelibNumber>::lessThan;
    auto encrypted_isMatch =  FHEUtils<SimplifiedHelibNumber>::areEqualBinary;

    std::vector<int> ones_data = VectorUtils::generate_binary_std_vector(size, number_of_matches_in_database);
    std::vector<int> zeros_data = VectorUtils::generate_binary_std_vector(size, size-number_of_matches_in_database);
    std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(size, number_of_matches_in_database, arbitrary_lookup_value);

    int number_of_batches = ceil( ((double) number_of_matches_in_database / batch_size));

    int min_database_size = 2048;
    int max_database_size = 20000;
    std::vector<std::unique_ptr<TestCase>> test_cases = generate_plain_secure_report_tests(min_database_size, max_database_size ,one_lookup_value, number_of_matches_in_database, generic_isMatch);

    // test_cases.push_back(move(test_plain_secure_report_protocol(one_lookup_value, size, number_of_matches_in_database, generic_isMatch, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_report_protocol(one_lookup_value, size, number_of_matches_in_database, encrypted_isMatch, ones_data)));
    // test_cases.push_back(move(test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, batch_size_number_of_matches, size, number_of_matches_in_database, generic_isMatch, ones_data)));
    // test_cases.push_back(move(test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, all_matches, size, number_of_matches_in_database, generic_isMatch, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, batch_size_number_of_matches_in_database, size, number_of_matches_in_database, encrypted_isMatch, ones_data)));
    // test_cases.push_back(move(test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, all_matches, size, number_of_matches_in_database, encrypted_isMatch, ones_data)));

    string log_name = "run_test";
    ProtocolTester tester(log_name);
    tester.initialize(test_cases);
    tester.test_and_log_all();
}

// TODO:
// 1. Finish implement ProtocolTestCase class for SBR - retrieval by batches
// 2. Implement In Protocol.cpp, SBR protocol, functionality to 1. retrieve batch, 2. retrieve all batches, 3. get number of batches 
// 3. Test cases builder class