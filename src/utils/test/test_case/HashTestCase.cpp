//
// Created by gil on 28.11.2020.
//

#include "HashTestCase.h"


HashFunctionTestCase::HashFunctionTestCase(unique_ptr<HashFunctionFamily>& hash_function, int database_size, int number_of_matches,
                                           double average_batch_size):
        _hash_function(move(hash_function)),
        _database_size(database_size),
        _number_of_matches(number_of_matches),
        _number_of_batches(get_number_of_batches(average_batch_size, number_of_matches)),
        _average_batch_size(average_batch_size) {}


unique_ptr<Data> HashFunctionTestCase::test() {
    auto domain_word_length = get_word_length(_database_size);
    auto number_of_batches_word_length = get_word_length(_number_of_batches);
   _hash_function->initialize(domain_word_length, number_of_batches_word_length);
    auto indices_indicator = sample_indices(_number_of_matches, _database_size);

    auto start_construction_time = high_resolution_clock::now();
    _hash_function->build();
    auto end_construction_time = high_resolution_clock::now();
    auto construction_time = end_construction_time - start_construction_time;
    long construction_time_in_nanoseconds = duration_cast<nanoseconds>(construction_time).count();

    auto start_build_time = high_resolution_clock::now();
    _hash_function->evaluate_all_domain();
    auto end_build_time = high_resolution_clock::now();
    auto build_time = end_build_time - start_build_time;
    long build_time_in_nanoseconds = duration_cast<nanoseconds>(build_time).count();

    auto start_evaluation_time = high_resolution_clock::now();
    auto evaluated_subset = _hash_function->evaluate_subset(indices_indicator);
    auto end_evaluation_time = high_resolution_clock::now();
    auto evaluation_time = end_evaluation_time - start_evaluation_time;
    long evaluation_time_in_nanoseconds = duration_cast<nanoseconds>(evaluation_time).count();

    unique_ptr<Data> hash_data_ptr(new HashFunctionData(_hash_function, _number_of_matches,
                                                        evaluated_subset, construction_time_in_nanoseconds, build_time_in_nanoseconds,
                                                        evaluation_time_in_nanoseconds));

    return hash_data_ptr;
}

string HashFunctionTestCase::to_string() {
    string message = "Hash name:" + _hash_function.get()->get_function_name() + "\n" +
            "Database size:" + std::to_string(_database_size) + "\n" +
            "Matches:" + std::to_string(_number_of_matches) + "\n" +
            "Batches:" + std::to_string(_number_of_batches) + "\n" +
            "Average batch size:" + std::to_string(_average_batch_size) + "\n";
    return message;
}

int HashFunctionTestCase::get_number_of_batches(double average_batch_size, int number_of_matches) {
    int number_of_batches = (int)(number_of_matches/average_batch_size);
    return number_of_batches;
}

int HashFunctionTestCase::get_word_length(int number) {
    int word_length = ceil(log2(number));
    return word_length;
}

 std::vector<int> HashFunctionTestCase::sample_indices(int number_of_indices, int range_upper_bound) {
    auto sampled_indices = VectorUtils::sample_random_ints(number_of_indices, range_upper_bound-1);
    return sampled_indices;
}
