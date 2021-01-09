//
// Created by gil on 28/04/20.
//

#include <iostream>

#include "GenericZP.h"
#include "SimplifiedHelibNumber.h"
#include "Protocol.h"


#define SIMD_FACTOR constants::WORD_LENGTH

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

template<class DataType>
void test_secure_report_protocol(int lookup_value,
                                          int database_size,
                                          int number_of_matches,
                                          DataType (*isMatch)(DataType&, DataType&),
                                          std::vector<int>& plain_data,
                                          unique_ptr<DatabaseDataType>& data_type_ptr) {

    SecureReportClient<DataType> client(data_type_ptr);
    SecureReportServer<DataType> server(database_size, number_of_matches);

    SecureReportProtocol<DataType> protocol(server, client);
    protocol.initialize();
    protocol.upload(plain_data, lookup_value);
    SecureReportQuery<DataType> query;
    query.initialize(lookup_value, isMatch);
    unique_ptr<PlainQuery<DataType>> query_ptr(new SecureReportQuery<DataType>(query));
    protocol.query(move(query_ptr));
    auto result = protocol.retrieve();
    cout << "Result: " << VectorUtils::std_vector_to_string(result) << "\n";
}

void test_plain_secure_report_protocol(int lookup_value,
                                                int database_size,
                                                int number_of_matches,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    unique_ptr<DatabaseDataType> data_type_ptr(new GenericPlainDataType(generic_plain_data_type));

    test_secure_report_protocol<GenericZP>(lookup_value, database_size,
                                                    number_of_matches, isMatch, plain_data, data_type_ptr);
}

void test_encrypted_secure_report_protocol(int lookup_value,
                                                    int database_size,
                                                    int number_of_matches,
                                                    SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                    std::vector<int>& plain_data) {

    unique_ptr<DatabaseDataType> data_type_ptr(new EncryptedDataTypeFromParameters(get_encrypted_data_type(database_size)));

    test_secure_report_protocol<SimplifiedHelibNumber>(lookup_value, database_size,
                                                                number_of_matches, isMatch, plain_data, data_type_ptr);
}


template<class DataType>
void test_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int batch_index,
                                                int database_size,
                                                int number_of_matches,
                                                DataType (*isMatch)(DataType&, DataType&),
                                                std::vector<int>& plain_data,
                                                unique_ptr<DatabaseDataType>& data_type_ptr) {

    SecureBatchRetrievalClient<DataType> client(data_type_ptr);
    SecureBatchRetrievalServer<DataType> server(database_size, number_of_matches);

    SecureBatchRetrievalProtocol<DataType> protocol(server, client);
    protocol.initialize();
    protocol.upload(plain_data, lookup_value);
    SecureBatchRetrievalQuery<DataType> query;
    query.initialize(lookup_value, batch_size, batch_index, database_size, number_of_matches, isMatch);
    unique_ptr<PlainQuery<DataType>> query_ptr(new SecureBatchRetrievalQuery<DataType>(query));
    protocol.query(move(query_ptr));
    auto result = protocol.retrieve();
    cout << "Result: " << VectorUtils::std_vector_to_string(result) << "\n";
}

void test_plain_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int batch_index,
                                                int database_size,
                                                int number_of_matches,
                                                GenericZP (*isMatch)(GenericZP&, GenericZP&),
                                                std::vector<int>& plain_data) {
    GenericPlainDataType generic_plain_data_type = get_plain_data_type();
    unique_ptr<DatabaseDataType> data_type_ptr(new GenericPlainDataType(generic_plain_data_type));

    test_secure_batch_retrieval_protocol<GenericZP>(lookup_value, batch_size, batch_index, database_size,
                                                    number_of_matches, isMatch, plain_data, data_type_ptr);
}

void test_encrypted_secure_batch_retrieval_protocol(int lookup_value,
                                                int batch_size,
                                                int batch_index,
                                                int database_size,
                                                int number_of_matches,
                                                SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber&, SimplifiedHelibNumber&),
                                                std::vector<int>& plain_data) {

    unique_ptr<DatabaseDataType> data_type_ptr(new EncryptedDataTypeFromParameters(get_encrypted_data_type(database_size)));

    test_secure_batch_retrieval_protocol<SimplifiedHelibNumber>(lookup_value, batch_size, batch_index, database_size,
                                                    number_of_matches, isMatch, plain_data, data_type_ptr);
}

int main(int argc, char** argv) {
    std::cout << "Current path is: " << fs::current_path() << "\n";
    int size = 2048;
    int number_of_matches = 4;
    int batch_size = 2;

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


    int number_of_batches = ceil( ((double) number_of_matches / batch_size));

    test_plain_secure_report_protocol(one_lookup_value, size, number_of_matches, generic_isMatch, ones_data);
    test_encrypted_secure_report_protocol(one_lookup_value, size, number_of_matches, encrypted_isMatch, ones_data);
    test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, 0, size, number_of_matches, generic_isMatch, ones_data);
    test_plain_secure_batch_retrieval_protocol(one_lookup_value, batch_size, 1, size, number_of_matches, generic_isMatch, ones_data);
    test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, 0, size, number_of_matches, encrypted_isMatch, ones_data);
    test_encrypted_secure_batch_retrieval_protocol(one_lookup_value, batch_size, 1, size, number_of_matches, encrypted_isMatch, ones_data);
}
