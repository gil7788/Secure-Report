//
// Created by gil on 1.10.2020.
//

#ifndef SECURE_REPORT_HASHFUNCTIONDATA_H
#define SECURE_REPORT_HASHFUNCTIONDATA_H

#include <string>

#include "HashFunctionsFamily.h"

using namespace std;

class Data {
public:
    virtual ~Data() {};
    
    virtual string to_json() = 0;

protected:
    string add_first_json_element(string name, string value);

    string add_middle_json_element(string name, string value);

    string add_last_json_element(string name, string value);

private:

    string add_json_element_content(string name, string value);
};

class HashFunctionData: public Data {
public:
    string _hash_name;
    int _number_of_matches;
    int _batches;
    long _domain_size;
    int _number_of_random_bits;
    double _average_batch_size;
    bool _results_are_set;
    long _construction_time;
    long _build_time;
    long _evaluation_time;
    double _mean_batch_size;
    double _batch_size_diviation;

public:
    HashFunctionData(unique_ptr<HashFunctionFamily>& hash, int number_of_matches);
    HashFunctionData(unique_ptr<HashFunctionFamily>& hash, int number_of_matches, vector<int> &evaluated_domain,
     long construction_time, long build_time, long evaluation_time);

    string to_json();

private:
    double get_mean_batch_size(vector<int>& split, int range_size);

    double get_batch_size_diviation(vector<int>& split, int range_size);

    vector<int> count_matches_per_batch(vector<int>& split, int range_size);

    double vector_mean(vector<int>& v);

    double diviation(vector<int>& v);

    string get_hash_name_key();

    string get_number_of_matches_key();

    string get_batches_key();

    string get_domain_size_key();

    string get_number_of_random_bits_key();

    string get_results_are_set_key();

    string get_average_batch_size_key();

    string get_construction_time_key();

    string get_build_time_key();

    string get_evaluation_time_key();

    string get_mean_batch_size_key();

    string get_batch_size_diviation_key();
};


#endif //SECURE_REPORT_HASHFUNCTIONDATA_H
