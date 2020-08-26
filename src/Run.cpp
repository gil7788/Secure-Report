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

template <class EncryptedQuery, typename DataType>
bool upload(Client<DataType>& client, Server<EncryptedQuery, DataType>& server, std::vector<int> plain_data, int comparison_value) {
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

template <class PlainQuery, class EncryptedQuery, typename DataType>
std::vector<int> retrieve(Client<DataType>& client, Server<EncryptedQuery, DataType>& server, TrustedThirdParty& trusted_third_party,
        PlainQuery query) {
    // Client
    EncryptedQuery encrypted_query = client.template query_server<PlainQuery, EncryptedQuery>(query);

    // Server
    std::vector<DataType> encrypted_matches_indices = server.send_matches_indices_to_client(encrypted_query, trusted_third_party);

    //Client
    std::vector<int> matches_indices = client.retrieve_matches_indices(encrypted_matches_indices, trusted_third_party);
    return matches_indices;
}

// =========== Secure Batch Retrieval =============
template <typename DataType>
void test_secure_batch_retrieval_client(Client<DataType>& client,
                                        Server<EncryptedSecureBatchRetrievalQuery<DataType>, DataType>& server,
                                        TrustedThirdParty& trusted_third_party,
                                        int lookup_value,
                                        int batch_size,
                                        int batch_index,
                                        int database_size,
                                        DataType (*isMatch)(DataType&, DataType&),
                                        std::vector<int>& plain_data) {
    bool uploaded_successful = upload<EncryptedSecureBatchRetrievalQuery<DataType>, DataType>(client, server, plain_data, lookup_value);

    if(! uploaded_successful) {
        std::cout << "Failed to upload data\n";
        return;
    }

    SecureBatchRetrievalQuery<DataType> query(lookup_value, batch_size, batch_index, database_size, isMatch);

    std::vector<int> matches_indices =
            retrieve<SecureBatchRetrievalQuery<DataType>, EncryptedSecureBatchRetrievalQuery<DataType>, DataType>(client, server, trusted_third_party, query);

    string result_string = VectorUtils::std_vector_to_string(matches_indices);
    std::cout << "Result: " << result_string << std::endl;
}

void test_generic_secure_batch_retrieval_client(int database_size,
                                                int sparsity,
                                                GenericPlainDataType& generic_plain_data_type,
                                                int lookup_value,
                                                int batch_size,
                                                int batch_index,
                                                GenericZP (*isMatch)(GenericZP &, GenericZP &),
                                                std::vector<int>& plain_data) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<GenericZP> client(database_size, sparsity, generic_plain_data_type);
    Server<EncryptedSecureBatchRetrievalQuery<struct GenericZP>, struct GenericZP> server(database_size, sparsity, generic_plain_data_type);

    test_secure_batch_retrieval_client<GenericZP>(client,
            server,
            trusted_third_party,
            lookup_value,
            batch_size,
            batch_index,
            database_size,
            isMatch,
            plain_data);
}

void test_encrypted_secure_batch_retrieval_client(int database_size,
                                                int sparsity,
                                                EncryptedDataTypeFromParameters& encrypted_plain_data_type,
                                                int lookup_value,
                                                int batch_size,
                                                int batch_index,
                                                SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber &, SimplifiedHelibNumber &),
                                                std::vector<int>& plain_data) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<SimplifiedHelibNumber> client(database_size, sparsity, encrypted_plain_data_type);
    Server<EncryptedSecureBatchRetrievalQuery<struct SimplifiedHelibNumber>, struct SimplifiedHelibNumber> server(database_size, sparsity, encrypted_plain_data_type);

    test_secure_batch_retrieval_client<SimplifiedHelibNumber>(client,
                                                  server,
                                                  trusted_third_party,
                                                  lookup_value,
                                                  batch_size,
                                                  batch_index,
                                                  database_size,
                                                  isMatch,
                                                  plain_data);
}

// =========== Secure Report ======================
template <typename DataType>
void test_secure_report_client(Client<DataType>& client, Server<EncryptedSecureReportQuery<DataType>, DataType>& server,
        TrustedThirdParty& trusted_third_party,
        int lookup_value, DataType (*isMatch)(DataType&, DataType&), std::vector<int>& plain_data) {
    bool uploaded_successful = upload<EncryptedSecureReportQuery<DataType>, DataType>(client, server, plain_data, lookup_value);

    if(! uploaded_successful) {
        std::cout << "Failed to upload data\n";
        return;
    }

    SecureReportQuery<DataType> query(lookup_value, isMatch);

    std::vector<int> matches_indices =
            retrieve<SecureReportQuery<DataType>, EncryptedSecureReportQuery<DataType>, DataType>(client, server, trusted_third_party, query);

    string result_string = VectorUtils::std_vector_to_string(matches_indices);
    std::cout << "Result: " << result_string << std::endl;
}

void test_generic_secure_report_client(int database_size, int sparsity, GenericPlainDataType& generic_plain_data_type,
                         int lookup_value, GenericZP (*isMatch)(GenericZP &, GenericZP &), std::vector<int>& plain_data) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<GenericZP> client(database_size, sparsity, generic_plain_data_type);
    Server<EncryptedSecureReportQuery<GenericZP>, GenericZP> server(database_size, sparsity, generic_plain_data_type);
    test_secure_report_client(client, server, trusted_third_party, lookup_value, isMatch, plain_data);
}

void test_encrypted_secure_report_client(int database_size, int sparsity, EncryptedDataTypeFromParameters& encrypted_data_type,
                           int lookup_value, SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber &, SimplifiedHelibNumber &), std::vector<int>& plain_data) {
    TrustedThirdParty trusted_third_party{database_size, sparsity};
    Client<SimplifiedHelibNumber> client(database_size, sparsity, encrypted_data_type);
    Server<EncryptedSecureReportQuery<SimplifiedHelibNumber>, SimplifiedHelibNumber> server(database_size, sparsity, encrypted_data_type);
    test_secure_report_client(client, server, trusted_third_party, lookup_value, isMatch, plain_data);
}

void serial_test(int database_size, int sparsity, EncryptedDataTypeFromParameters& encrypted_data_type,
                 int upper_bound, SimplifiedHelibNumber (*isMatch)(SimplifiedHelibNumber &, SimplifiedHelibNumber &)) {
    for(int i = 1; i < upper_bound; ++i) {
        cout << "Testing value " << i << "\n";
        std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(database_size, sparsity, i);
        test_encrypted_secure_report_client(database_size, sparsity, encrypted_data_type, i, isMatch, arbitrary_data);
    }
}

int main(int argc, char** argv) {
    std::cout << "Current path is: " << fs::current_path() << "\n";
    int size = 128;
    int sparsity = 3;

    long r = constants::WORD_LENGTH;
    int L = (int) ceil(log(r)) + 2;
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
    int arbitrary_lookup_value = 2;
    auto binary_arbitrary_lookup_value = VectorUtils::number_to_std_vector(arbitrary_lookup_value, (int) r);

    auto generic_isMatch =  FHEUtils<GenericZP>::areEqualBinary;
    auto generic_lessThan = FHEUtils<GenericZP>::lessThan;
    auto encrypted_lessThan = FHEUtils<SimplifiedHelibNumber>::lessThan;
    auto encrypted_isMatch =  FHEUtils<SimplifiedHelibNumber>::areEqualBinary;

    std::vector<int> ones_data = VectorUtils::generate_binary_std_vector(size, sparsity);
    std::vector<int> zeros_data = VectorUtils::generate_binary_std_vector(size, size-sparsity);
    std::vector<int> arbitrary_data = VectorUtils::generate_int_vector(size, sparsity, arbitrary_lookup_value);

    GenericPlainDataType generic_plain_data_type = GenericPlainDataType(r);
    EncryptedDataTypeFromParameters encrypted_data_type(size, s, R, r, d, c, k, L, chosen_m, gens, ords, key_file_path);

//    serial_test(size, sparsity, encrypted_data_type, 100, encrypted_isMatch);
//    test_generic_secure_report_client(size, sparsity, generic_plain_data_type, one_lookup_value,
//                        reinterpret_cast<GenericZP (*)(GenericZP &, GenericZP &, int)>(generic_isMatch), ones_data);
//    test_generic_secure_report_client(size, sparsity, generic_plain_data_type, zero_lookup_value,
//                        reinterpret_cast<GenericZP (*)(GenericZP &, GenericZP &, int)>(generic_isMatch), zeros_data);
    cout << "Plain areEquals: \n";
    test_generic_secure_report_client(size, sparsity, generic_plain_data_type, arbitrary_lookup_value, (generic_isMatch), arbitrary_data);
    cout << "Plain lessThan: \n";
    test_generic_secure_report_client(size, sparsity, generic_plain_data_type, arbitrary_lookup_value - 1, (generic_lessThan), arbitrary_data);
    cout << "Plain equals SBR: \n";
    test_generic_secure_batch_retrieval_client(size, sparsity, generic_plain_data_type, arbitrary_lookup_value,
            size, 0, generic_isMatch, arbitrary_data);

    cout << "Encrypted equals SBR: \n";
    test_encrypted_secure_batch_retrieval_client(size, sparsity, encrypted_data_type, arbitrary_lookup_value,
                                               size, 0, encrypted_isMatch, arbitrary_data);

//    test_encrypted_secure_report_client(size, sparsity, encrypted_data_type, one_lookup_value,
//                          reinterpret_cast<SimplifiedHelibNumber (*)(SimplifiedHelibNumber &, SimplifiedHelibNumber &,
//                                                                     int)>(encrypted_isMatch), ones_data);
//    test_encrypted_secure_report_client(size, sparsity, encrypted_data_type, zero_lookup_value,
//                          reinterpret_cast<SimplifiedHelibNumber (*)(SimplifiedHelibNumber &, SimplifiedHelibNumber &,
//                                                                     int)>(encrypted_isMatch), zeros_data);
//    cout << "Encrypted areEquals: \n";
//    test_encrypted_secure_report_client(size, sparsity, encrypted_data_type, arbitrary_lookup_value,
//                          reinterpret_cast<SimplifiedHelibNumber (*)(SimplifiedHelibNumber &, SimplifiedHelibNumber &,
//                                                                     int)>(encrypted_isMatch), arbitrary_data);
//    cout << "Encrypted lessThan: \n";
//    test_encrypted_secure_report_client(size, sparsity, encrypted_data_type, arbitrary_lookup_value - 1,
//                          reinterpret_cast<SimplifiedHelibNumber (*)(SimplifiedHelibNumber &, SimplifiedHelibNumber &,
//
//                                                                     int)>(encrypted_lessThan), arbitrary_data);
}
