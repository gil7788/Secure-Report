#include <experimental/filesystem>

#include "../HashFunctions/HashFunctionsFamily.h"
#include "../HashFunctions/TestHashFunctionFamily.h"
#include "../HashFunctions/SBRTestData.h"

namespace fs = std::experimental::filesystem;

// TODO: Either delete or add this method to each of the hash functions classes
double evaluate_probability(int number_of_matches, int batch_size_slackness,
        int independence) {
    auto summand_1 = 2 * number_of_matches * independence/ pow(batch_size_slackness, 2);
    auto summand_2 = number_of_matches / batch_size_slackness;
    double probability = pow(summand_1, independence) + pow(summand_2, 2*independence);
    return probability;
}

template<typename HASH_TYPE>
vector<unique_ptr<TestCase>> construct_tests(int database_size,
                                    vector<int>& number_of_matches, vector<int>& batches_average_sizes) {
    vector<unique_ptr<TestCase>> tests;
    for(auto average_batch_size: batches_average_sizes){
        for (auto matches: number_of_matches) {
            unique_ptr<HashFunctionFamily> hash_function(new HASH_TYPE());
//            HashFunctionTestCase test(reinterpret_cast<unique_ptr<HashFunctionFamily> &>(hash_function), database_size, matches, average_batch_size);
             unique_ptr<TestCase> test_case(new HashFunctionTestCase(hash_function, database_size, matches, average_batch_size));
             tests.push_back(move(test_case));
            // // SBRTestData test_case(hash_function, database_size, matches, average_batch_size);
            // tests.push_back(test_case);
            // cout << test_case.to_string() << "\n =================================\n\n";
        }
    }
    return tests;
}

// vector<SBRTestData> construct_tests(HashFunctionFamily& hash_function, int database_size,
//                                     vector<int>& number_of_matches, vector<int>& batches_average_sizes) {
//     vector<SBRTestData> tests;
//     for(auto average_batch_size: batches_average_sizes){
//         for (auto matches: number_of_matches) {
//             SBRTestData test_case(hash_function, database_size, matches, average_batch_size);
//             tests.push_back(test_case);
//             cout << test_case.to_string() << "\n =================================\n\n";
//         }
//     }
//     return tests;
// }

// vector<SBRTestData> construct_tests(HashFunctionFamily& hash_function, int database_size,
//                                     vector<int>& number_of_matches, vector<vector<int>>& batches_average_sizes_options) {
//     vector<SBRTestData> tests;
//     for (int i = 0; i < number_of_matches.size(); ++i) {
//         auto batches_average_sizes = batches_average_sizes_options[i];
//         for(auto& average_batch_size: batches_average_sizes) {
//             SBRTestData test_case(hash_function, database_size, number_of_matches[i], average_batch_size);
//             tests.push_back(test_case);
//             cout << test_case.to_string() << "\n =================================\n\n";
//         }
//     }
//     return tests;
// }

// void test_hash_function(vector<SBRTestData>& tests, TestHashFunctionFamily& tester) {
//     for(auto& test: tests) {
//         test.test(tester);
//     }
// }

int round_to_upper_power_of_two(int number) {
    int number_of_bits = int(ceil(log2(number)));
    return int(pow(2, number_of_bits));
}

template<typename HASH_TYPE>
void run_test_suit(string& log_name, int database_size, vector<int>& number_of_matches, vector<int>& batches_average_sizes_options) {
    TestHashFunctionFamily tester(log_name);
    auto tests = construct_tests<HASH_TYPE>(database_size, number_of_matches, batches_average_sizes_options);
    tester.initialize(tests);
    tester.test_and_log_all();
}

int main(int argc, char** argv) {
//    int database_size = round_to_upper_power_of_two(10000000);
//    int database_size = round_to_upper_power_of_two(100000);
    int database_size = round_to_upper_power_of_two(1000);

    int matches_upper_bound = database_size/2;
//    int matches_upper_bound = round_to_upper_power_of_two(500);

    int lowest_match = matches_upper_bound;
    vector<int> number_of_matches = {lowest_match, 2*lowest_match, 4*lowest_match};

    vector<int> batches_average_sizes{2,4,8,16};
    unique_ptr<HashFunctionFamily> trivial_hash(new TrivialHashFunctionsFamily());
    string log_name = "trivial_hash";
    run_test_suit<TrivialHashFunctionsFamily>(log_name, database_size, number_of_matches, batches_average_sizes);

//    unique_ptr<HashFunctionFamily> tabulation_hash(new Tabulation);
//    run_test_suit(tabulation_hash, database_size, number_of_matches, batches_average_sizes);

//    TwistedTabulation twisted_hash;
//    run_test_suit(twisted_hash, database_size, number_of_matches, batches_average_sizes);

// TODO implement polynomial hash constructor from independence
    // unique_ptr<HashFunctionFamily> polynomial_hash(new PolynomialHashFunctionsFamily());
    // int independence = 4;
    // polynomial_hash->set_independence(independence);
    // run_test_suit(polynomial_hash, database_size, number_of_matches, batches_average_sizes);

//    unique_ptr<HashFunctionFamily> gradually_increasing_hash(new GraduallyIncreasingHashFunctionsFamily());
//    run_test_suit(gradually_increasing_hash, database_size, number_of_matches, batches_average_sizes);

//    NisanGenerator nisan_generator;
//    run_test_suit(nisan_generator, database_size, number_of_matches, batches_average_sizes);
}
