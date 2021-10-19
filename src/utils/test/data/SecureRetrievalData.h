//
// Created by gil on 10.1.2021.
//

#ifndef SECURE_SECURERETRIEVALDATA_H
#define SECURE_SECURERETRIEVALDATA_H

#include <iostream>
#include "../../../FHEDatabaseConfig.h"
#include "Data.h"
#include "TestResult.h"

using namespace std;

template<class DataType>
class SecureRetrievalData: public Data {
public:
    const string _name;
    int _lookup_value;
    int _database_size;
    int _number_of_matches_in_database;
    unique_ptr<RetrievalTestResult<int>> _test_result;


    DataType (*_isMatch)(DataType&, DataType&);
    vector<int> _plain_data;
    unique_ptr<DatabaseDataType>& _data_type_ptr;
    bool _results_are_set;
    long _upload_time;
    long _initialize_time;
    long _retrieval_time;
    JsonParser parser;
    
    SecureRetrievalData(const string& name, int lookup_value, int database_size, int number_of_matches_in_database,
                        vector<int>& selected_matches_indices, vector<int>& evaluated_matches_indices,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr,
                        long upload_time, long initialize_time, long retrieval_time):
                        _name{name},
                        _lookup_value{lookup_value},
                        _database_size{database_size},
                        _number_of_matches_in_database{number_of_matches_in_database},
                        _test_result{initialize_test_result(selected_matches_indices, evaluated_matches_indices, database_size)},
                        _isMatch{isMatch}, _plain_data(plain_data), _data_type_ptr(data_type_ptr), _results_are_set{true},
                        _upload_time{upload_time}, _initialize_time{initialize_time}, _retrieval_time(retrieval_time) {}

    virtual string to_json() override{
        JsonParser parser = build_json_parser();
        string json = parser.to_json();
        return json;
    };

    virtual JsonParser build_json_parser() {
        JsonParser parser;
        parser.add_value("name",  _name);
        parser.add_value("lookup_value",  to_string(_lookup_value));
        parser.add_value("database_size", to_string(_database_size));
        parser.add_value("number_of_matches_in_database", to_string(_number_of_matches_in_database));
        parser.add_value("test_result", _test_result->to_json());
        parser.add_value("initialize_time", to_string(_initialize_time));
        parser.add_value("upload_time", to_string(_upload_time));
        parser.add_value("retrieval_time", to_string(_retrieval_time));
        return parser;
    }

private:
    unique_ptr<RetrievalTestResult<int>> initialize_test_result(vector<int> selected_matches_indices, vector<int> evaluated_matches_indices, int database_size) {
        unique_ptr<RetrievalTestResult<int>> test_result = make_unique<RetrievalTestResult<int>>(RetrievalTestResult<int>(selected_matches_indices, evaluated_matches_indices, database_size));
        return test_result;
    }
};

template<class DataType>
class SecureReportData: public SecureRetrievalData<DataType> {
public:
    SecureReportData(const string& name, int lookup_value,int database_size, int number_of_matches_in_database, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time):
                        SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_matches_in_database, number_of_retrieved_matches,
                                      isMatch, plain_data, data_type_ptr, upload_time, initialize_time, retrieval_time) {}
};

template<class DataType>
class SecureBatchRetrievalData: public SecureRetrievalData<DataType> {
    int _batch_size;
    int _number_of_requested_matches;

public:
    SecureBatchRetrievalData(const string& name, int lookup_value,int database_size, int number_of_matches_in_database, int number_of_requested_matches, vector<int> selected_matches_indices, vector<int> evaluated_matches_indices,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time, 
                        int batch_size):  _batch_size{batch_size}, _number_of_requested_matches{number_of_requested_matches},
                          SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_matches_in_database, selected_matches_indices, evaluated_matches_indices,
                                      isMatch, plain_data, data_type_ptr, upload_time, initialize_time, retrieval_time) {}

    JsonParser build_json_parser() override{
        JsonParser parser = SecureRetrievalData<DataType>::build_json_parser();
        parser.add_value("batch_size", to_string(_batch_size));
        return parser;
    }

private:
// TODO: implement, probabliy pass _number_of retrieved batches to Data class
    int get_number_of_retrieved_batches() {
        int number_of_retrieved_batches = 0;
        return number_of_retrieved_batches;
    }
};


#endif //SECURE_SECURERETRIEVALDATA_H
