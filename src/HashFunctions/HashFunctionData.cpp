//
// Created by gil on 1.10.2020.
//

#include "HashFunctionData.h"


string Data::add_first_json_element(string name, string value) {
    string json = "{\n" + add_json_element_content(name, value) + ",\n";
    return json;
}

string Data::add_middle_json_element(string name, string value) {
    string json = add_json_element_content(name, value) +  ",\n";
    return json;
}

string Data::add_last_json_element(string name, string value) {
    string json = add_json_element_content(name, value) + "}\n";
    return json;
}

string Data::add_json_element_content(string name, string value) {
    return "\"" + name + "\": \"" + value + "\"";
}

string HashFunctionData::to_json() {
    string json = add_first_json_element(get_hash_name_key(), _hash_name);
    json += add_middle_json_element(get_batches_key(), to_string(_batches));
    json += add_middle_json_element(get_domain_size_key(), to_string(_domain_size));
    json += add_middle_json_element(get_number_of_random_bits_key(), to_string(_number_of_random_bits));
    json += add_middle_json_element(get_number_of_matches_key(), to_string(_number_of_matches));
    json += add_middle_json_element(get_results_are_set_key(), to_string(_results_are_set));
    json += add_middle_json_element(get_average_batch_size_key(), to_string(_average_batch_size));
    json += add_middle_json_element(get_construction_time_key(), to_string(_construction_time));
    json += add_middle_json_element(get_build_time_key(), to_string(_build_time));
    json += add_middle_json_element(get_evaluation_time_key(), to_string(_evaluation_time));
    json += add_middle_json_element(get_mean_batch_size_key(), to_string(_mean_batch_size));
    json += add_last_json_element(get_batch_size_diviation_key(), to_string(_batch_size_diviation));

    return json;
}

HashFunctionData::HashFunctionData(unique_ptr<HashFunctionFamily>& hash, int number_of_matches):
    _hash_name{hash->get_function_name()},
    _batches{hash->get_range_size()},
    _domain_size{hash->get_domain_size()},
    _number_of_random_bits{hash->get_number_of_random_bits()},
    _number_of_matches{number_of_matches},
    _average_batch_size{double(double(number_of_matches)/hash->get_range_size())},
    _results_are_set{false}
{}

HashFunctionData::HashFunctionData(unique_ptr<HashFunctionFamily>& hash,
 int number_of_matches, vector<int> &evaluated_domain, long construction_time, long build_time, long evaluation_time):
    _hash_name{hash->get_function_name()},
    _batches{hash->get_range_size()},
    _domain_size{hash->get_domain_size()},
    _number_of_random_bits{hash->get_number_of_random_bits()},
    _number_of_matches{number_of_matches},
    _average_batch_size{double(double(number_of_matches)/hash->get_range_size())},
    _results_are_set{true},
     _construction_time{construction_time},
    _build_time{build_time},
    _evaluation_time{evaluation_time},
    _mean_batch_size{get_mean_batch_size( evaluated_domain, _batches)},
    _batch_size_diviation{get_batch_size_diviation(evaluated_domain, _batches)}
{}

//void HashFunctionData::set_running_results(vector<int> &evaluated_domain, long construction_time, long build_time, long evaluation_time) {
//    _construction_time = construction_time;
//    _build_time = build_time;
//    _evaluation_time = evaluation_time;
//    _mean_batch_size = get_mean_batch_size( evaluated_domain, _batches);
//    _batch_size_diviation = get_batch_size_diviation(evaluated_domain, _batches);
//    _results_are_set = true;
//}

double HashFunctionData::get_mean_batch_size(vector<int> &split, int range_size) {
    auto number_of_matches_per_batch = count_matches_per_batch(split, range_size);
    auto mean_batch_size = vector_mean(number_of_matches_per_batch);
    return mean_batch_size;
}

double HashFunctionData::get_batch_size_diviation(vector<int> &split, int range_size) {
    auto number_of_matches_per_batch = count_matches_per_batch(split, range_size);
    auto batch_size_diviation = diviation(number_of_matches_per_batch);
    return batch_size_diviation;
}

vector<int> HashFunctionData::count_matches_per_batch(vector<int>& split, int range_size) {
    vector<int> number_of_matches_per_batch(range_size, 0);
    for(auto& value: split) {
        number_of_matches_per_batch[value] += 1;
    }
    return number_of_matches_per_batch;
}

double HashFunctionData::vector_mean(vector<int>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    return mean;
}

double HashFunctionData::diviation(vector<int>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();

    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
    return stdev;
}

string HashFunctionData::get_hash_name_key() {
    string key = "hash_name";
    return key;
}

string HashFunctionData::get_number_of_matches_key() {
    string key = "matches";
    return key;
}

string HashFunctionData::get_batches_key() {
    string key = "batches";
    return key;
}

string HashFunctionData::get_domain_size_key() {
    string key = "domain_size";
    return key;
}

string HashFunctionData::get_number_of_random_bits_key() {
    string key = "number_of_random_bits";
    return key;
}

string HashFunctionData::get_results_are_set_key() {
    string key = "results_are_set";
    return key;
}

string HashFunctionData::get_average_batch_size_key() {
    string key = "average_batch_size";
    return key;
}

string HashFunctionData::get_construction_time_key() {
    string key = "construction_time";
    return key;
}

string HashFunctionData::get_build_time_key() {
    string key = "build_time";
    return key;
}

string HashFunctionData::get_evaluation_time_key() {
    string key = "evaluation_time";
    return key;
}

string HashFunctionData::get_mean_batch_size_key() {
    string key = "mean_batch_size";
    return key;
}

string HashFunctionData::get_batch_size_diviation_key() {
    string key = "batch_size_diviation";
    return key;
}
