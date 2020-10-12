//
// Created by gil on 4.9.2020.
//

#include "TestHashFunctionFamily.h"
#include "HashFunctionData.h"

void TestHashFunctionFamily::try_test_hash_function_and_log(HashFunctionFamily& hash_function, int domain_word_length,
                                                            int number_of_batches_word_length, int number_of_matches_word_length) {
    try {
        test_single_instance(hash_function, domain_word_length, number_of_batches_word_length,
                                    number_of_matches_word_length);
    }
    catch (const std::exception& e) {
        int number_of_matches = pow(2, number_of_matches_word_length);
        HashFunctionData hash_data(hash_function, number_of_matches);

        string error_string = "Failed to test hash function " + hash_data.to_json() + "\n";

    }
    log_result();
}

void TestHashFunctionFamily::test_hash_function(HashFunctionFamily &hash_function, vector<int>& indices_indicator,
                                                  int number_of_matches) {
    auto start_construction_time = high_resolution_clock::now();
    hash_function.build();
    auto end_construction_time = high_resolution_clock::now();
    auto construction_time = end_construction_time - start_construction_time;
    long construction_time_in_nanoseconds = duration_cast<nanoseconds>(construction_time).count();
    // TODO consider to delete
    Duration build_time_duration = Duration(construction_time);

    auto start_build_time = high_resolution_clock::now();
    hash_function.evaluate_all_domain();
    auto end_build_time = high_resolution_clock::now();
    auto build_time = end_build_time - start_build_time;
    long build_time_in_nanoseconds = duration_cast<nanoseconds>(build_time).count();

    auto start_evaluation_time = high_resolution_clock::now();
    auto evaluated_subset = test_subset(hash_function, indices_indicator);
    auto end_evaluation_time = high_resolution_clock::now();
    auto evaluation_time = end_evaluation_time - start_evaluation_time;
    long evaluation_time_in_nanoseconds = duration_cast<nanoseconds>(evaluation_time).count();
    // TODO consider to delete
    Duration evaluation_time_duration = Duration(evaluation_time);

    HashFunctionData hash_data(hash_function, number_of_matches);
    hash_data.set_running_results(evaluated_subset, construction_time_in_nanoseconds, build_time_in_nanoseconds,
            evaluation_time_in_nanoseconds);
    _data.add_data(hash_data);
}
bool TestHashFunctionFamily::log_result() {
    bool log_saved = _data.save_log();
    return log_saved;
}
//string TestHashFunctionFamily::log_result(HashFunctionFamily& hash, int number_of_matches, vector<int> &evaluated_domain,
//                                          long construction_time, long evaluation_time) {
//
//    string output = hash_function.to_string();
//    output += "Construction Time: " + build_time_duration.to_string() + "\n";
//    output += "Evaluation Time: " + evaluation_time_duration.to_string() + "\n";
//    output += "Mean batch size: " + std::to_string(vector_mean(matches_per_batch)) + "\n";
//    output += "Batch size diviation: " + std::to_string(diviation(matches_per_batch)) + "\n";
//    return output;
//}

vector<int> TestHashFunctionFamily::test_subset(HashFunctionFamily& hash_function, vector<int>& subset) {
    auto evaluated_subset = hash_function.evaluate_subset(subset);
    return evaluated_subset;
}

void TestHashFunctionFamily::test_single_instance(HashFunctionFamily& hash_function, int domain_word_length,
        int number_of_batches_word_length, int number_of_matches_word_length) {
    long number_of_matches = pow(2, number_of_matches_word_length);
    long domain_size = pow(2, domain_word_length);

    hash_function.initialize(domain_word_length, number_of_batches_word_length);
    auto indices = sample_indices(number_of_matches, domain_size);
    test_hash_function(hash_function, indices, number_of_matches);
}

std::vector<int> TestHashFunctionFamily::sample_indices(int number_of_indices, int range_upper_bound) {
    auto sampled_indices = VectorUtils::sample_random_ints(number_of_indices, range_upper_bound);
    return sampled_indices;

//    // TODO add random selection
//    std::vector<int> indices;
//    for( int i = 0; i < number_of_indices; i++)
//        indices.push_back(i);
//
//    return indices;
}

Duration::Duration(std::chrono::duration<double> duration) {
    normalize_duration(duration);
}

void Duration::normalize_duration(std::chrono::duration<double> duration) {
    long NANOSECONDS_IN_MILLISECONDS = 1000000;
    long MILISECONDS_IN_SECOND = 1000;
    long SECONDS_IN_MINUTE = 60;
    long MINUTES_IN_HOUR = 60;

    // get duration in seconds
    double time_duration_in_nanoseconds = duration_cast<nanoseconds>(duration).count();
    if (time_duration_in_nanoseconds > (NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND * SECONDS_IN_MINUTE * MINUTES_IN_HOUR)) {
        _duration = duration_cast<hours>(duration).count();
        _time_unit = TIME_UNIT::H;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND * SECONDS_IN_MINUTE) {
        _duration = duration_cast<minutes>(duration).count();
        _time_unit = TIME_UNIT::M;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND) {
        _duration = duration_cast<seconds>(duration).count();
        _time_unit = TIME_UNIT::S;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS) {
        _duration = duration_cast<milliseconds>(duration).count();
        _time_unit = TIME_UNIT::MS;
    }
    else {
        _duration = time_duration_in_nanoseconds;
        _time_unit = TIME_UNIT::NS;
    }
}

string Duration::to_string() {
    ostringstream ss;
    ss << *this;
    return ss.str();
}

ostream& operator<<(ostream& os, const Duration& duration) {
    string unit_string;
    switch (duration._time_unit) {
        case Duration::TIME_UNIT::NS:
            unit_string = "ns";
            break;

        case Duration::TIME_UNIT::MS:
            unit_string = "ms";
            break;

        case Duration::TIME_UNIT::S:
            unit_string = "s";
            break;

        case Duration::TIME_UNIT::M:
            unit_string = "m";
            break;

        case Duration::TIME_UNIT::H:
            unit_string = "h";
            break;
    }
    os << duration._duration << " " << unit_string;
    return os;
}
