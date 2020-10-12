#include <experimental/filesystem>
#include <range.h>

#include "../HashFunctions/HashFunctionsFamily.h"
#include "../HashFunctions/TestHashFunctionFamily.h"
#include "../InputOutput.h"
#include "../Config.h"

namespace fs = std::experimental::filesystem;

// TODO: Either delete or add this method to each of the hash functions classes
double evaluate_probability(int number_of_matches, int batch_size_slackness,
        int independence) {
    auto summand_1 = 2 * number_of_matches * independence/ pow(batch_size_slackness, 2);
    auto summand_2 = number_of_matches / batch_size_slackness;
    double probability = pow(summand_1, independence) + pow(summand_2, 2*independence);
    return probability;
}

void test_hash_function(HashFunctionFamily& hash_function, int database_size,
        int range_word_length_lower_bound = 1) {
    TestHashFunctionFamily tester;
    int database_word_length = ceil(log2(database_size));
    int maximal_number_of_matches = pow(ceil(log10(database_size)), 3);
    int maximal_number_of_matches_word_length = ceil(log2(maximal_number_of_matches));
    int maximal_number_of_batches_word_length = maximal_number_of_matches_word_length;

    for(auto number_of_batches_word_length: range(range_word_length_lower_bound, maximal_number_of_matches_word_length)) {
        for(auto number_of_matches_word_length: range(number_of_batches_word_length, maximal_number_of_batches_word_length)) {
            tester.try_test_hash_function_and_log(hash_function, database_word_length, number_of_batches_word_length,
                                                  number_of_matches_word_length);
        }
    }
}

void test_k_wise_hash_function(KWiseIndependentHashFunctionFamily& hash_function, int database_size,
        int independence) {
    hash_function.set_independence(independence);
    test_hash_function(hash_function, database_size);
}

int main(int argc, char** argv) {
    int database_size = 10000000;
//    int database_size = 1000;

    TestHashFunctionFamily tester;

    TrivialHashFunctionsFamily hash;
    Tabulation tabulation_hash;
    TwistedTabulation twisted_hash;
    GraduallyIncreasingHashFunctionsFamily gradually_increasing_hash;
    PolynomialHashFunctionsFamily polynomial_hash;
    int independence = 4;
    NisanGenerator nisan_generator;

//    Done
//    test_hash_function(hash, database_size);
//    test_k_wise_hash_function(polynomial_hash, database_size, independence);
//    Done
//    test_hash_function(twisted_hash, database_size, independence);
    test_hash_function(tabulation_hash, database_size);
//    Done
//    test_hash_function(gradually_increasing_hash, database_size, 5);
//    test_hash_function(nisan_generator, database_size);
}
