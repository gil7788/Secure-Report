//
// Created by gil on 10.1.2021.
//

#ifndef SECURE_SECURERETRIEVALDATA_H
#define SECURE_SECURERETRIEVALDATA_H

#include <iostream>
#include "../../../FHEDatabaseConfig.h"
#include "Data.h"

using namespace std;

template<class DataType>
class SecureRetrievalData: public Data {
public:
    int _lookup_value;
    int _database_size;
    int _number_of_matches;
    DataType (*_isMatch)(DataType&, DataType&);
    vector<int> _plain_data;
    unique_ptr<DatabaseDataType>& _data_type_ptr;
    bool _results_are_set;
    long _upload_time;
    long _initialize_time;
    long _retrieval_time;

    SecureRetrievalData(int lookup_value, int database_size, int number_of_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr):
                        _lookup_value{lookup_value},
                        _database_size{database_size}, _number_of_matches{number_of_matches},
                        _isMatch{isMatch}, _plain_data(plain_data), _data_type_ptr(data_type_ptr), _results_are_set{false} {}
    
    SecureRetrievalData(int lookup_value, int database_size, int number_of_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr,
                        long upload_time, long initialize_time, long retrieval_time):
                        _lookup_value{lookup_value},
                        _database_size{database_size}, _number_of_matches{number_of_matches},
                        _isMatch{isMatch}, _plain_data(plain_data), _data_type_ptr(data_type_ptr), _results_are_set{true},
                        _upload_time{upload_time}, _initialize_time{initialize_time}, _retrieval_time{retrieval_time} {}

//    TODO: Add more variables
    string to_json() {
        string json = add_first_json_element(string("lookup_value"),  to_string(_lookup_value));
        json += this->add_middle_json_element(string("database_size"), to_string(_database_size));
        json += this->add_middle_json_element(string("number_of_matches"), to_string(_number_of_matches));
        json += this->add_middle_json_element(string("initialize_time"), to_string(_initialize_time));
        json += this->add_middle_json_element(string("upload_time"), to_string(_upload_time));
        json += this->add_last_json_element(string("retrieval_time"), to_string(_retrieval_time));
        return json;
    };
};

template<class DataType>
class SecureBatchRetrievalData: public SecureRetrievalData<DataType> {
    int _batch_size;
    int _batch_index;

public:
    SecureBatchRetrievalData(int lookup_value,int database_size, int number_of_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, int batch_size, int batch_index):
                        _batch_size{batch_size}, _batch_index{batch_index},
                        SecureRetrievalData<DataType>(lookup_value, database_size, number_of_matches,
                                      isMatch, plain_data, data_type_ptr) {}

    SecureBatchRetrievalData(int lookup_value,int database_size, int number_of_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time, 
                        int batch_size, int batch_index):
                        _batch_size{batch_size}, _batch_index{batch_index},
                        SecureRetrievalData<DataType>(lookup_value, database_size, number_of_matches,
                                      isMatch, plain_data, data_type_ptr, upload_time, initialize_time, retrieval_time) {}

    string to_json() {
        string json = this->add_first_json_element("lookup_value", to_string(this->_lookup_value));
        json += this->add_middle_json_element("database_size", to_string(this->_database_size));
        json += this->add_middle_json_element("number_of_matches", to_string(this->_number_of_matches));
        json += this->add_middle_json_element("initialize_time", to_string(this->_initialize_time));
        json += this->add_middle_json_element("upload_time", to_string(this->_upload_time));
        json += this->add_middle_json_element("retrieval_time", to_string(this->_retrieval_time));
        json += this->add_middle_json_element("batch_size", to_string(_batch_size));
        json += this->add_last_json_element("batch_index", to_string(_batch_index));
        return json;
    };
};


#endif //SECURE_SECURERETRIEVALDATA_H
