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

fs::path build_log_file_path(string& hash_function_name) {
    auto hash_function_name_with_underscore = hash_function_name;
    std::replace( hash_function_name_with_underscore.begin(), hash_function_name_with_underscore.end(), ' ', '_' );
    string file_name = hash_function_name_with_underscore + "_log";
    auto file_path = fs::path(constants::PROCESSED_FILE_PATH);
    file_path.concat(file_name);
    return file_path;
}

void test_hash_function(HashFunctionFamily& hash_function, string& hash_function_name, int database_size,
        int range_word_length_lower_bound = 1) {

    fs::path file_path = build_log_file_path(hash_function_name);

    InputOutput io(constants::OUTPUT_TO_CONSOLE, file_path.u8string(), constants::OUTPUT_LEVEL);
    string output = "Test " + hash_function_name + ": \n";
    output += "Database size: " + std::to_string(database_size) + "\n";
    output += "-------------------------------\n";

    TestHashFunctionFamily tester;
    int database_word_length = ceil(log2(database_size));
    int maximal_number_of_matches = pow(ceil(log10(database_size)), 3);
    int maximal_number_of_matches_word_length = ceil(log2(maximal_number_of_matches));
    int maximal_number_of_batches_word_length = maximal_number_of_matches_word_length;

    for(auto number_of_matches_word_length: range(range_word_length_lower_bound+1, maximal_number_of_matches_word_length)) {
        for(auto number_of_batches_word_length: range(number_of_matches_word_length, maximal_number_of_batches_word_length)) {
            int number_of_batches = ceil(pow(2, number_of_batches_word_length));
            try {
                output += tester.test_single_instance(hash_function, database_word_length, number_of_matches_word_length,
                                                      number_of_batches);
            }
            catch (const std::exception& e) {
                string error_string = "Failed to test hash function \n" + hash_function.to_string();
                output += error_string;
            }
            io.output(output, constants::OUTPUT_LEVELS::LOG);
        }
    }
}

void test_k_wise_hash_function(KWiseIndependentHashFunctionFamily& hash_function, string& hash_function_name, int database_size,
        int independence) {
    hash_function.set_independence(independence);
    test_hash_function(hash_function, hash_function_name, database_size);
}

int main(int argc, char** argv) {
    int database_size = 10000000;

//    TrivialHashFunctionsFamily trivial_hash_function;
//    string trivial_hash_function_name = "Trivial Hash Function Family";
//    test_hash_function(trivial_hash_function, trivial_hash_function_name, database_size);
//
//    PolynomialHashFunctionsFamily polynomial_hash_function;
//    string polynomial_hash_function_name = "Polynomial Hash Function Family";
//    test_k_wise_hash_function(polynomial_hash_function, polynomial_hash_function_name, database_size, 4);
//
    GraduallyIncreasingHashFunctionsFamily increasing_hash_function;
    string increasing_hash_function_name = "Increasing Hash Function Family";
    int increasing_hash_function_lower_bound = 4;
    test_hash_function(increasing_hash_function, increasing_hash_function_name, database_size, increasing_hash_function_lower_bound);

//    Tabulation tabulation_hash;
//    string tabulation_hash_function_name = "Tabulation Hash Function Family";
//    test_hash_function(tabulation_hash, tabulation_hash_function_name, database_size);

//    TwistedTabulation twisted_hash;
//    string twisted_hash_hash_function_name = "Twisted Tabulation Hash Function Family";
//    test_hash_function(twisted_hash, twisted_hash_hash_function_name, database_size, 8);

//    NisanGenerator nisan_generator;
//    nisan_generator.initialize(4, 2);
}
