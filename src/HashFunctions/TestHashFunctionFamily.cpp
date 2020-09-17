//
// Created by gil on 4.9.2020.
//

#include "TestHashFunctionFamily.h"

vector<int> TestHashFunctionFamily::get_batches_size(vector<int>& split, int number_of_matches) {
    vector<int> number_of_matches_sizes(number_of_matches, 0);
    for(auto& value: split) {
        number_of_matches_sizes[value] += 1;
    }
    return number_of_matches_sizes;
}

double TestHashFunctionFamily::vector_mean(vector<int>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    return mean;
}

double TestHashFunctionFamily::diviation(vector<int>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();

    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
    return stdev;
}

string TestHashFunctionFamily::test_hash_function(HashFunctionFamily &hash_function, vector<int>& indices_indicator) {
    auto start_build_time = high_resolution_clock::now();
    hash_function.build();
    auto end_build_time = high_resolution_clock::now();
    auto construction_time = end_build_time - start_build_time;
    Duration build_time_duration = Duration(construction_time);

    auto start_evaluation_time = high_resolution_clock::now();
    auto evaluated_subset = test_subset(hash_function, indices_indicator);
    auto end_evaluation_time = high_resolution_clock::now();
    auto evaluation_time = end_evaluation_time - start_evaluation_time;
    Duration evaluation_time_duration = Duration(construction_time);

    return log_result(hash_function, evaluated_subset, build_time_duration, evaluation_time_duration);
}

string TestHashFunctionFamily::log_result(HashFunctionFamily& hash_function, vector<int> &evaluated_domain,
                                          Duration& build_time_duration, Duration& evaluation_time_duration) {

    int range = pow(2, hash_function.get_range_word_length());

    auto number_of_matches_sizes = get_batches_size(evaluated_domain, range);
    vector<int> un_empty_bathes;
    for (int i = 0; i < number_of_matches_sizes.size(); ++i) {
        if(number_of_matches_sizes[i] != 0)
            un_empty_bathes.push_back(number_of_matches_sizes[i]);
    }

    string output = hash_function.to_string();
    output += "Construction Time: " + build_time_duration.to_string() + "\n";
    output += "Evaluation Time: " + evaluation_time_duration.to_string() + "\n";
    output += "Mean batch size: " + std::to_string(vector_mean(un_empty_bathes)) + "\n";
    output += "Batch size diviation: " + std::to_string(diviation(un_empty_bathes)) + "\n";
    return output;
}

vector<int> TestHashFunctionFamily::test_subset(HashFunctionFamily& hash_function, vector<int>& subset) {
    auto evaluated_subset = hash_function.evaluate_subset(subset);
    return evaluated_subset;
}

string TestHashFunctionFamily::test_single_instance(HashFunctionFamily& hash_function, int domain_word_length,
        int number_of_batches_word_length, int number_of_matches) {

    int number_of_batches = pow(2, number_of_batches_word_length);
    string output = "Number of matches: " + std::to_string(number_of_matches) + "\n";
    output += "Number of batches: " + std::to_string(number_of_batches) + "\n";

    hash_function.initialize(domain_word_length, number_of_batches_word_length);
    auto indices = sample_indices(number_of_matches);
    output += test_hash_function(hash_function, indices);
    output += "===============================\n";
    return output;
}

std::vector<int> TestHashFunctionFamily::sample_indices(int number_of_indices) {
    std::vector<int> indices;
    for( int i = 0; i < number_of_indices; i++)
        indices.push_back(i);

    return indices;
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
