#include "../HashFunctions/HashFunctionsFamily.h"
#include "../HashFunctions/TestHashFunctionFamily.h"


std::vector<int> sample_indices(int number_of_indices) {
    std::vector<int> indices;
    for( int i = 0; i < number_of_indices; i++)
        indices.push_back(i);

    return indices;
}

double evaluate_probability(int number_of_matches, int batch_size_slackness,
        int independence) {
    auto summand_1 = 2 * number_of_matches * independence/ pow(batch_size_slackness, 2);
    auto summand_2 = number_of_matches / batch_size_slackness;
    double probability = pow(summand_1, independence) + pow(summand_2, 2*independence);
    return probability;
}

int main(int argc, char** argv) {
    int maximal_number_of_matches = 300;
    int batch_size_upper_bound = 10;
    int batch_size_average = 8;
    int batch_size_slackness = batch_size_upper_bound - batch_size_average;
    double probability = 0.5;

    int domain_word_length = 20;
    int range_word_length = ceil(log2(maximal_number_of_matches / batch_size_upper_bound));
    int independence = range_word_length;

    TwistedTabulation twisted_hash;
    twisted_hash.initialize(16, 8);

    Tabulation tabulation_hash;
    tabulation_hash.initialize(16, 2);
//    tabulation_hash.initialize(domain_word_length, range_word_length);

    NisanGenerator nisan_generator;
    nisan_generator.initialize(4, 2);

    TrivialHashFunctionsFamily trivial_hash_function;
    trivial_hash_function.initialize(domain_word_length, range_word_length);
    PolynomialHashFunctionsFamily polynomial_hash_function;
    polynomial_hash_function.initialize(domain_word_length, range_word_length, independence);
    GraduallyIncreasingHashFunctionsFamily increasing_hash_function;
    increasing_hash_function.initialize(domain_word_length, range_word_length);

    auto indices = sample_indices(maximal_number_of_matches);
    auto generator_indices = sample_indices(16);
    TestHashFunctionFamily tester;

    cout << "Tester: \n";
    cout << "Nisan Generator \n";
    tester.test_hash_function(nisan_generator, generator_indices, batch_size_upper_bound);
    cout << "Twisted Tabulation \n";
    tester.test_hash_function(twisted_hash, indices, batch_size_upper_bound);
    cout << "Tabulation \n";
    tester.test_hash_function(tabulation_hash, indices, batch_size_upper_bound);
//    tabulation_hash.build();
//    VectorXi evaluated_subset = tabulation_hash.evaluate_all_domain();
//    auto vstd = VectorUtils::eigen_vector_to_std_vector(evaluated_subset);
//    tester.log_result(tabulation_hash, vstd);
    cout << "=====================================================================\n";
    cout << "Simple 2-wise independence: \n";
    tester.test_hash_function(trivial_hash_function, indices, batch_size_upper_bound);
    cout << "=====================================================================\n";
    cout << "Polynomial: \n";

    cout << "Probability: " << evaluate_probability(maximal_number_of_matches, batch_size_slackness, independence) << "\n";
    tester.test_hash_function(polynomial_hash_function, indices, batch_size_upper_bound);
    cout << "=====================================================================\n";
    cout << "Increasing: \n";
    tester.test_hash_function(increasing_hash_function, indices, batch_size_upper_bound);
}
