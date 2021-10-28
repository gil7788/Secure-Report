#ifndef TEST_RESULT_H
#define TEST_RESULT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "../../../infrastructure/JsonParser.h"

using namespace std;

/**
 * Measure success percentage of test result
 */
class TestResult {
public:
    virtual ~TestResult() {};

    virtual string to_json() = 0;

    virtual int get_number_of_false_positive() = 0;

    virtual int get_number_of_false_negative() = 0;

    virtual int get_number_of_true_positive() = 0;

    virtual int get_number_of_true_negative() = 0;
};


template<class DataType>
class RetrievalTestResult: public TestResult {
    vector<DataType> _selected_matches;
    vector<DataType> _evaluated_matches;
    int _database_size;

public:
    RetrievalTestResult(vector<DataType> selected_matches, vector<DataType> evaluated_matches, int database_size):
     _selected_matches{selected_matches}, _evaluated_matches{evaluated_matches}, _database_size{database_size} {}


    double get_retrieval_precentage() {
        double precentage = get_number_of_selected_matches() / get_number_of_evaluated_matches();
        return precentage;
    }
    
    int get_number_of_selected_matches() {
        return _selected_matches.size();
    }

    int get_number_of_evaluated_matches() {
        return _evaluated_matches.size();
    }
    
    int get_number_of_false_positive() override{
        auto subtraction = subtract(_evaluated_matches, _selected_matches);
        return subtraction.size();
    }

    int get_number_of_false_negative() override{
        auto subtraction = subtract(_selected_matches, _evaluated_matches);
        return subtraction.size();
    }

    int get_number_of_true_positive() override{
        auto intersection = intersect(_selected_matches, _evaluated_matches);
        return intersection.size();
    }

    int get_number_of_true_negative() override{
        auto symmetric_difference_set_size = _database_size -
         (get_number_of_false_positive() + get_number_of_false_negative() + get_number_of_true_positive());
        return symmetric_difference_set_size;
    }

    string to_json() override {
        JsonParser parser;
        parser.add_value("number_of_false_positive", to_string(get_number_of_false_positive()));
        parser.add_value("number_of_false_negative", to_string(get_number_of_false_negative()));
        parser.add_value("number_of_true_positive", to_string(get_number_of_true_positive()));
        parser.add_value("number_of_true_negative", to_string(get_number_of_true_negative()));
        string json = parser.to_json();
        return json;
    }

    vector<DataType> intersect(vector<DataType> v1, vector<DataType> v2) {
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
    
        std::vector<DataType> v_intersection;
    
        std::set_intersection(v1.begin(), v1.end(),
                            v2.begin(), v2.end(),
                            std::back_inserter(v_intersection));

        return v_intersection;
    }

    vector<DataType> subtract(vector<DataType> v1, vector<DataType> v2) {
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());
    
        std::vector<DataType> v_subtraction;
    
        std::set_difference(v1.begin(), v1.end(),
                            v2.begin(), v2.end(),
                            std::back_inserter(v_subtraction));

        return v_subtraction;
    }
};



#endif //SECURE_DATA_H
