//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONSFAMILY_H
#define SECURE_REPORT_HASHFUNCTIONSFAMILY_H

#include <iostream>
#include <random>
#include <Eigen/Dense>
#include <experimental/filesystem>

#include "../VectorUtils.h"
#include "../InputOutput.h"
#include "../Config.h"

using Eigen::MatrixXi;
using Eigen::VectorXi;
using Eigen::Map;


using namespace std;
namespace fs = std::experimental::filesystem;

class HashFunctionFamily {
    int _domain_word_size;
    int _range_word_size;
    vector<int> _evaluated_domain;

protected:
    virtual double tests_bit_randomness(int number_of_tests);

    virtual int sample_random_bit();

    virtual int get_value_in_range(int x);

public:
    virtual ~HashFunctionFamily() = default;

    virtual int get_domain_word_length();

    virtual int get_range_word_length();

    virtual int get_domain_size();

    virtual int get_range_size();

    virtual int get_number_of_random_bits() = 0;

    int get_evaluated_value(int i);

    VectorXi get_evaluated_domain();

    void set_evaluated_domain(vector<int>& evaluated_domain);

    void set_evaluated_domain(VectorXi& evaluated_domain);

    void clear_evaluated_domain();

    virtual string to_string();

    fs::path build_log_file_path();

public:

    virtual void initialize(int domain_word_size, int range_word_size);

    virtual void build() = 0;

    virtual void evaluate_all_domain() = 0;

    vector<int> evaluate_subset(vector<int> &values_indices) ;

    virtual string get_function_name() = 0;

    friend ostream& operator<<(ostream& os, HashFunctionFamily& duration);

    bool write_evaluated_domain_to_file();
};

class TrivialHashFunctionsFamily: public HashFunctionFamily {
    MatrixXi _sampled_matrix;

public:
    int get_number_of_random_bits() override;

    void initialize(int domain_word_size, int range_word_size) override;

    void build() override ;

    void evaluate_all_domain() override;

    string get_function_name() override;

    int evaluate_value(int x);
};

class KWiseIndependentHashFunctionFamily:public HashFunctionFamily {
    int _k;

public:
    virtual void initialize(int domain_word_size, int range_word_size, int independence);

    virtual int get_independence();

    void set_independence(int k);

    friend ostream& operator<<(ostream& os, KWiseIndependentHashFunctionFamily& hash_function);
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
    int get_number_of_random_bits() override;

    void build() override;

    void evaluate_all_domain() override;

    string get_function_name() override;
};

class GraduallyIncreasingHashFunctionsFamily: public HashFunctionFamily {
    vector<int> _independence_degrees;
    vector<int> _domains;
    vector<int> _domains_word_length;
    vector<PolynomialHashFunctionsFamily> _hash_functions;

private:
    void build_matrices();

    void initialize_matrices();

public:
    int get_number_of_random_bits() override;

    void initialize(int domain_word_length, int range_word_length) override ;

    void build() override;

    void evaluate_all_domain() override;

    int evaluate_value(int value);

    string get_function_name() override;
};

class Tabulation: public HashFunctionFamily {
private:
    vector<TrivialHashFunctionsFamily> _hash_tables;
    VectorXi _tabulation_table;
    int _number_of_hash_tables;

public:
    int get_number_of_random_bits() override;

    void initialize(int domain_word_size, int range_word_size) override ;

    virtual void build() override;

    void evaluate_all_domain() override ;

    virtual int get_number_of_hash_tables();

    int get_word_length();

    VectorXi evaluate(int i);

    string get_function_name() override;
};

class TwistedTabulation: public Tabulation {
private:
    Tabulation _twister;
    Tabulation _simple_tabulation;

public:
    int get_number_of_random_bits() override;

    void initialize(int domain_word_size, int range_word_size) override;

    virtual void build() override;

    int get_word_length();

    void evaluate_all_domain();

    VectorXi evaluate(int i);

    string get_function_name() override;
};

class NisanGenerator: public HashFunctionFamily{
private:
    using HashFunctionFamily::initialize;
    vector<TrivialHashFunctionsFamily> _pair_wise_indpendent_hash_funtions;

    vector<long> evaluate_value(int value);

public:
    int get_number_of_random_bits() override;

    void initialize(int word_length, int k);

    void build();

    void evaluate_all_domain();

    string get_function_name() override;
};
#endif //SECURE_REPORT_HASHFUNCTIONSFAMILY_H
