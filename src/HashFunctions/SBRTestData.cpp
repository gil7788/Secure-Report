//
// Created by gil on 28.11.2020.
//

#include "SBRTestData.h"

SBRTestData::SBRTestData(HashFunctionFamily &hash_function, int database_size, int number_of_matches,
                         double average_batch_size):
                         _hash_function(hash_function),
                         _database_size(database_size),
                         _number_of_matches(number_of_matches),
                         _number_of_batches(get_number_of_batches(average_batch_size, number_of_matches)),
                         _average_batch_size(average_batch_size)
                         {}

int SBRTestData::get_number_of_batches(double average_batch_size, int number_of_matches) {
    int number_of_batches = (int)(number_of_matches/average_batch_size);
    return number_of_batches;
}

void SBRTestData::test(TestHashFunctionFamily& tester) {
    int database_word_length = get_word_length(_database_size);
    int matches_word_length = get_word_length(_number_of_matches);
    int batches_word_length = get_word_length(_number_of_batches);

    tester.try_test_hash_function_and_log(_hash_function, database_word_length, matches_word_length,
                                          batches_word_length);
}

int SBRTestData::get_word_length(int number) {
    int word_length = ceil(log2(number));
    return word_length;
}

string SBRTestData::to_string() {
    string message = "Hash name:" + _hash_function.get_function_name() + "\n" +
            "Database size:" + std::to_string(_database_size) + "\n" +
            "Matches:" + std::to_string(_number_of_matches) + "\n" +
            "Batches:" + std::to_string(_number_of_batches) + "\n" +
            "Average batch size:" + std::to_string(_average_batch_size) + "\n";
    return message;
}
