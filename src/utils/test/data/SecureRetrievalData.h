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
    const string _name;
    int _lookup_value;
    int _database_size;
    int _number_of_retrieved_matches;

    DataType (*_isMatch)(DataType&, DataType&);
    vector<int> _plain_data;
    unique_ptr<DatabaseDataType>& _data_type_ptr;
    bool _results_are_set;
    long _upload_time;
    long _initialize_time;
    long _retrieval_time;

    SecureRetrievalData(const string& name, int lookup_value, int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr):
                        _name{name},
                        _lookup_value{lookup_value},
                        _database_size{database_size}, _number_of_retrieved_matches{number_of_retrieved_matches},
                        _isMatch{isMatch}, _plain_data(plain_data), _data_type_ptr(data_type_ptr), _results_are_set{false} {}
    
    SecureRetrievalData(const string& name, int lookup_value, int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr,
                        long upload_time, long initialize_time, long retrieval_time):
                        _name{name},
                        _lookup_value{lookup_value},
                        _database_size{database_size}, _number_of_retrieved_matches{number_of_retrieved_matches},
                        _isMatch{isMatch}, _plain_data(plain_data), _data_type_ptr(data_type_ptr), _results_are_set{true},
                        _upload_time{upload_time}, _initialize_time{initialize_time}, _retrieval_time{retrieval_time} {}

//    TODO: Add more variables
    string to_json() {
        string json = add_first_json_element(string("name"),  _name);
        json += this->add_middle_json_element(string("lookup_value"),  to_string(_lookup_value));
        json += this->add_middle_json_element(string("database_size"), to_string(_database_size));
        json += this->add_middle_json_element(string("number_of_retrieved_matches"), to_string(_number_of_retrieved_matches));
        json += this->add_middle_json_element(string("initialize_time"), to_string(_initialize_time));
        json += this->add_middle_json_element(string("upload_time"), to_string(_upload_time));
        json += this->add_last_json_element(string("retrieval_time"), to_string(_retrieval_time));
        return json;
    };
};

template<class DataType>
class SecureReportData: public SecureRetrievalData<DataType> {
public:
    SecureReportData(const string& name, int lookup_value,int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr):
                        SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_retrieved_matches,
                                      isMatch, plain_data, data_type_ptr) {}

    SecureReportData(const string& name, int lookup_value,int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time):
                        SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_retrieved_matches,
                                      isMatch, plain_data, data_type_ptr, upload_time, initialize_time, retrieval_time) {}
};

template<class DataType>
class SecureBatchRetrievalData: public SecureRetrievalData<DataType> {
    int _batch_size;
    int _number_of_retrieved_matches;

public:
    SecureBatchRetrievalData(const string& name, int lookup_value,int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, int batch_size):
                        _batch_size{batch_size}, _number_of_retrieved_matches{number_of_retrieved_matches},
                        SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_retrieved_matches,
                                      isMatch, plain_data, data_type_ptr) {}

    SecureBatchRetrievalData(const string& name, int lookup_value,int database_size, int number_of_retrieved_matches,
                        DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time, 
                        int batch_size):
                        _batch_size{batch_size}, _number_of_retrieved_matches{number_of_retrieved_matches},
                        SecureRetrievalData<DataType>(name, lookup_value, database_size, number_of_retrieved_matches,
                                      isMatch, plain_data, data_type_ptr, upload_time, initialize_time, retrieval_time) {}

    string to_json() {
        string json = this->add_first_json_element(string("name"), this->_name);
        json += this->add_middle_json_element("lookup_value", to_string(this->_lookup_value));
        json += this->add_middle_json_element("database_size", to_string(this->_database_size));
        json += this->add_middle_json_element("number_of_retrieved_matches", to_string(this->_number_of_retrieved_matches));
        json += this->add_middle_json_element("initialize_time", to_string(this->_initialize_time));
        json += this->add_middle_json_element("upload_time", to_string(this->_upload_time));
        json += this->add_middle_json_element("retrieval_time", to_string(this->_retrieval_time));
        json += this->add_middle_json_element("batch_size", to_string(_batch_size));
        json += this->add_middle_json_element("number_of_retrieved_matches", to_string(_number_of_retrieved_matches));
        json += this->add_last_json_element("number_of_retrieved_batches", to_string(get_number_of_retrieved_batches()));
        return json;
    };

private:
    int get_number_of_retrieved_batches() {
        int number_of_retrieved_batches = int(ceil(_number_of_retrieved_matches/_batch_size));
        return number_of_retrieved_batches;
    }
};


#endif //SECURE_SECURERETRIEVALDATA_H
