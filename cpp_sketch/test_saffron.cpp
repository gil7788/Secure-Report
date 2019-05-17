#include <iostream>
#include <ctime>
#include <Eigen/Dense>

#include "sketch_encoder.h"

using Eigen::MatrixXi;
using Eigen::VectorXi;


std::vector<int> getMatches(VectorXi x) {
    std::vector<int> indices;
    for (int i = 0; i < x.size(); ++i) {
        if(x[i] == 1) {
            indices.insert(indices.begin(), i);
        }
    }
    return indices;
}

bool saffron_test(int n, int d) {
    sketch_encoder sketch_test(n, d);

    // Init input
    VectorXi input;
    input= VectorXi::Zero(n);
    input = sketch_test.generate_binary_vector(n, d);
    std::vector<int> indices = getMatches(input);

    double encode_duration;
    std::clock_t start = std::clock();

    // Encode
    VectorXi encoded = sketch_test.encode(input);
    encode_duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"encode_duration: "<< encode_duration <<'\n';

    double decode_duration;
    start = std::clock();
    // Decode
    VectorXi decoded = sketch_test.decode(encoded);
    decode_duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"decode_duration: "<< decode_duration <<'\n';


    std::vector<int> decoded_std(decoded.data(), decoded.data() + decoded.rows() * decoded.cols());

    std::cout << "decoded_std: " << std::endl;
    for (auto i = decoded_std.begin(); i != decoded_std.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;

    std::cout << "input: " << std::endl;
    for (auto i = indices.begin(); i != indices.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;

    bool is_successful_decode(std::is_permutation(indices.begin(), indices.end(), decoded_std.begin()));

    return is_successful_decode;
}

int main() {
    int power_upper_bound = 12;
    int power_lower_bound = 10;
    int d_upper_bound = 12;
    int d_lower_bound = 2;

    int tests_successful(0);
    int tests(0);


    for (int j = d_lower_bound; j < d_upper_bound; ++j) {
        for (int i = power_lower_bound; i < power_upper_bound; ++i) {
            int n = int(std::pow(2, i));
            int d = j;
            if(saffron_test(n, d)) {
                tests_successful += 1;
            }
            tests += 1;
        }
    }
    double ratio(double(tests_successful/tests));
    std::cout << "tests_successful: " << tests_successful<< std::endl;
    std::cout << "tests: " << tests << std::endl;
    std::cout << "ratio: " << ratio << std::endl;
}
