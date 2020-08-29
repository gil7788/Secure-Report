//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONSFAMILY_H
#define SECURE_REPORT_HASHFUNCTIONSFAMILY_H

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include "VectorUtils.h"


using Eigen::MatrixXi;
using Eigen::VectorXi;


using namespace std;

class HashFunctionsFamily {
    int _domain_word_size;
    int _range_word_size;
    MatrixXi _sampledMatrix;

private:
    double tests_bit_randomness(int number_of_tests) {
        double average = 0;
        for (int n=0; n < number_of_tests; ++n) {
            auto sampled = sample_random_bit();
            average += sampled;
        }
        average = average / number_of_tests;
        return average;
    }

    int sample_random_bit() {
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, 1);
        auto sampled_bit = distrib(gen);
        return sampled_bit;
    }

public:
    HashFunctionsFamily(int domain_word_size, int range_word_size):
    _domain_word_size{domain_word_size},
    _range_word_size{range_word_size},
//    _sampledMatrix(domain_word_size, range_word_size)
        _sampledMatrix{MatrixXi::NullaryExpr(_domain_word_size, _range_word_size, [&](){return sample_random_bit();})}
    {
//        for (int i = 0; i < _domain_word_size; ++i) {
//            for (int j = 0; j < _range_word_size; ++j) {
//                _sampledMatrix(i,j) = sample_random_bit();
//            }
//        }
    };

    int evaluate(int x) {
        auto x_binary = VectorUtils::number_to_binary(x, _domain_word_size);
        auto x_evaluation_binary = x_binary.transpose() * _sampledMatrix;

        auto x_evaluation = VectorUtils::binary_to_number(x_evaluation_binary);
        return x_evaluation;
    }

    vector<int> evaluate_all_domain() {
        vector<int> range;
        int domain_size = pow(2, _domain_word_size);
        for (int i = 0; i < domain_size; ++i) {
            auto range_at_i = evaluate(i);
            range.push_back(range_at_i);
        }
        return range;
    }
};


#endif //SECURE_REPORT_HASHFUNCTIONSFAMILY_H
