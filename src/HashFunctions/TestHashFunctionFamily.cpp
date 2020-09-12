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

void TestHashFunctionFamily::test_hash_function(HashFunctionFamily &hash_function, vector<int>& indices_indicator,
                                                int batch_size_upper_bound) {
    hash_function.build();
    auto evaluated_subset = test_subset(hash_function, indices_indicator);
    log_result(hash_function, evaluated_subset);
}

void TestHashFunctionFamily::log_result(HashFunctionFamily& hash_function, vector<int> &evaluated_domain) {
    int domain = pow(2, hash_function.get_domain_word_length());
    int range = pow(2, hash_function.get_range_word_length());

    auto number_of_matches_sizes = get_batches_size(evaluated_domain, range);
    vector<int> un_empty_bathes;
    for (int i = 0; i < number_of_matches_sizes.size(); ++i) {
        if(number_of_matches_sizes[i] != 0)
            un_empty_bathes.push_back(number_of_matches_sizes[i]);
    }

    cout << "Number of batch: " << un_empty_bathes.size() << "\n";
    cout << "Evaluated Domain: " << VectorUtils::std_vector_to_string(evaluated_domain) << "\n";
    cout << "Un_empty_bathes Domain: " << VectorUtils::std_vector_to_string(un_empty_bathes) << "\n";
    cout << "mean: " << vector_mean(un_empty_bathes) << "\n";
    cout << "diviation: " << diviation(un_empty_bathes) << "\n";
}

vector<int> TestHashFunctionFamily::test_subset(HashFunctionFamily& hash_function, vector<int>& subset) {
    auto evaluated_subset = hash_function.evaluate_subset(subset);
    return evaluated_subset;
}
