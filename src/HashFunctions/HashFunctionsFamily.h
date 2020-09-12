//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONSFAMILY_H
#define SECURE_REPORT_HASHFUNCTIONSFAMILY_H

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include "../VectorUtils.h"

using Eigen::MatrixXi;
using Eigen::VectorXi;
using Eigen::Map;


using namespace std;
class HashFunctionFamily {
    int _domain_word_size;
    int _range_word_size;
    MatrixXi _sampledMatrix;

protected:
    virtual double tests_bit_randomness(int number_of_tests);

    virtual int sample_random_bit();

    virtual int get_value_in_range(int x);

public:

    virtual int get_domain_word_length();

    virtual int get_range_word_length();

    virtual int get_domain_size();

    virtual int get_range_size();
public:

    virtual void initialize(int domain_word_size, int range_word_size);

    virtual void build();

    virtual vector<int> evaluate_subset(vector<int> &values_indices) = 0;

    virtual VectorXi evaluate_all_domain()  = 0;
};

class TrivialHashFunctionsFamily: public HashFunctionFamily {
    MatrixXi _sampledMatrix;

public:
    void initialize(int domain_word_size, int range_word_size) override;

    virtual vector<int> evaluate_subset(vector<int> &values_indices);

    VectorXi evaluate_all_domain() override;

    int evaluate_value(int x);
};

class KWiseIndependentHashFunctionFamily:public HashFunctionFamily {
    int _k;

public:
    virtual void initialize(int domain_word_size, int range_word_size, int k) = 0;

    virtual int get_independence();
private:
    using HashFunctionFamily::initialize;
};

class PolynomialHashFunctionsFamily: public KWiseIndependentHashFunctionFamily {
    MatrixXi _vandermonde_matrix;
    VectorXi _polynom;
    MatrixXi _domain_matrix;

private:
    MatrixXi build_vandermonde_matrix();

    void build_domain_matrix();

    void sample_polynom();

    vector<int> get_vandermonde_elements(int number_of_element);

    VectorXi  sample_seed();
public:
    void initialize(int domain_word_size, int range_word_size, int k) override;

    void build() override;

    VectorXi evaluate_all_domain() override;

    MatrixXi evaluate_all_domain_binary();

    vector<int> evaluate_subset(vector<int>& values_subset) override;
};

class GraduallyIncreasingHashFunctionsFamily: public HashFunctionFamily {
    vector<int> _independence_degrees;
    vector<int> _domains;
    vector<int> _domains_word_length;
    vector<PolynomialHashFunctionsFamily> _hash_functions;
    vector<int> _evaluated_domain;

private:
    vector<MatrixXi> build_vandermonde_matrices();

    void build_matrices();

    void initialize_matrices();

    vector<int> get_evaluated_domain();
public:
    void build() override;

    // TODO fix returns type. Make private.
    VectorXi evaluate_all_domain() override;

    int evaluate_value(int value);

    vector<int> evaluate_subset(vector<int>& values_subset) override ;
};

class Tabulation: public HashFunctionFamily {
private:
    vector<PolynomialHashFunctionsFamily> _hash_tables;
    VectorXi _tabulation_table;
    int _number_of_hash_tables;

public:
    void initialize(int domain_word_size, int range_word_size) override ;

    virtual void build() override;

    virtual int get_number_of_hash_tables();

    virtual int get_word_length();

    VectorXi evaluate(int i);

    vector<int> evaluate_subset(vector<int> &values_indices) override ;

    VectorXi evaluate_all_domain();
};

class TwistedTabulation: public Tabulation {
private:
    Tabulation _twister;
    Tabulation _simple_tabulation;
    VectorXi _evaluated_domain;

public:
    void initialize(int domain_word_size, int range_word_size) override;

    virtual void build() override;

    VectorXi evaluate(int i);

    vector<int> evaluate_subset(vector<int> &values_indices) override;

    VectorXi evaluate_all_domain();
};

class NisanGenerator: public HashFunctionFamily{
private:
    using HashFunctionFamily::initialize;
    vector<TrivialHashFunctionsFamily> _pair_wise_indpendent_hash_funtions;
    VectorXi _evaluated_domain;

    vector<long> evaluate_value(int value);
public:
    void initialize(int word_length, int k);

    void build();

    VectorXi evaluate_all_domain();

    vector<int> evaluate_subset(vector<int> &values_indices);
};
#endif //SECURE_REPORT_HASHFUNCTIONSFAMILY_H
