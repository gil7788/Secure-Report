//
// Created by gil on 10.1.2021.
//

#ifndef SECURE_SECURERETRIEVALTESTCASE_H
#define SECURE_SECURERETRIEVALTESTCASE_H

#include <iostream>
#include <memory>

#include "../data/Data.h"
#include "../data/SecureRetrievalData.h"
#include "TestCase.h"
#include "../../../Client.h"
#include "../../../Server.h"
#include "../../../Protocol.h"

#include <chrono>
using namespace std::chrono;


template<class DataType>
class SecureRetrievalTestCase: public TestCase {
public:
    int _lookup_value;
    int _database_size;
    int _number_of_matches;
    DataType (*_is_match)(DataType&, DataType&);
    vector<int> _plain_data;
    unique_ptr<DatabaseDataType> _data_type_ptr;

    SecureRetrievalTestCase(int lookup_value,int database_size, int number_of_matches,
                        DataType (*is_match)(DataType&, DataType&), vector<int>& plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr):
            _lookup_value{lookup_value},
            _database_size{database_size}, _number_of_matches{number_of_matches},
            _is_match{is_match}, _plain_data{plain_data}, _data_type_ptr(move(data_type_ptr)) {}

    virtual string to_string() override {
        string str = "lookup_value = " + std::to_string(get_lookup_value());
        str += "database_size = " + std::to_string(get_database_size());
        str += "number_of_matches = " + std::to_string(get_number_of_matches());
        return str;
    }

    virtual unique_ptr<Data> test() override {
        SecureReportClient<DataType> client(_data_type_ptr);
        SecureReportServer<DataType> server(_database_size, _number_of_matches);
        SecureReportProtocol<DataType> protocol(server, client);

        auto start_initialize_time = high_resolution_clock::now();
        protocol.initialize();
        auto end_initialize_time = high_resolution_clock::now();
        auto initialize_time = end_initialize_time - start_initialize_time;
        long initialize_time_in_nanoseconds = duration_cast<nanoseconds>(initialize_time).count();

        auto plain_data = this->_plain_data;

        auto start_upload_time = high_resolution_clock::now();
        protocol.upload(this->_plain_data, this->_lookup_value);
        auto end_upload_time = high_resolution_clock::now();
        auto upload_time = end_upload_time - start_upload_time;
        long upload_time_in_nanoseconds = duration_cast<nanoseconds>(upload_time).count();

        SecureReportQuery<DataType> query;
        query.initialize(this->_lookup_value, this->_is_match);
        unique_ptr<PlainQuery<DataType>> query_ptr(new SecureReportQuery<DataType>(query));
        protocol.query(move(query_ptr));

        auto start_retrieval_time = high_resolution_clock::now();
        auto result = protocol.retrieve();
        auto end_retrieval_time = high_resolution_clock::now();
        auto retrieval_time = end_retrieval_time - start_retrieval_time;
        long retrieval_time_in_nanoseconds = duration_cast<nanoseconds>(retrieval_time).count();

        // TODO consider to delete
        cout << "Result: " << VectorUtils::std_vector_to_string(result) << "\n";

        unique_ptr<Data> protocol_data_ptr(new SecureRetrievalData<DataType>(_lookup_value, _database_size, _number_of_matches,
         _is_match, _plain_data, _data_type_ptr, initialize_time_in_nanoseconds, upload_time_in_nanoseconds, retrieval_time_in_nanoseconds));
        
        return protocol_data_ptr;
    }

protected:

    int get_lookup_value() {
        return _lookup_value;
    }

    int get_database_size() {
        return _database_size;
    }

    int get_number_of_matches() {
        return _number_of_matches;
    }

    DataType (*get_is_match())(DataType&, DataType&) {
        return _is_match;
    }

    vector<int> get_plain_data() {
        return _plain_data;
    }

    unique_ptr<DatabaseDataType>& get_data_type_ptr() {
        return _data_type_ptr;
    }
};

template<class DataType>
class SecureBatchRetrievalTestCase: public SecureRetrievalTestCase<DataType> {
    int _batch_size;
    int _batch_index;

public:
    SecureBatchRetrievalTestCase(int lookup_value,int database_size, int number_of_matches,
                            DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                            unique_ptr<DatabaseDataType>& data_type_ptr, int batch_size, int batch_index):
            _batch_size{batch_size}, _batch_index{batch_index}, SecureRetrievalTestCase<DataType>(lookup_value, database_size, number_of_matches,
                                          isMatch, plain_data, data_type_ptr) {}

    string to_string()  override{
        string str = SecureRetrievalTestCase<DataType>::to_string();
        str += "_batch_size = " + std::to_string(get_batch_size());
        str += "_batch_index = " + std::to_string(get_batch_index());
        return str;
    }

//    TODO: Implement batch retrieval testing
    unique_ptr<Data> test() override {
        SecureReportClient<DataType> client(this->_data_type_ptr);
        SecureReportServer<DataType> server(this->_database_size, this->_number_of_matches);
        SecureReportProtocol<DataType> protocol(server, client);

        auto start_initialize_time = high_resolution_clock::now();
        protocol.initialize();
        auto end_initialize_time = high_resolution_clock::now();
        auto initialize_time = end_initialize_time - start_initialize_time;
        long initialize_time_in_nanoseconds = duration_cast<nanoseconds>(initialize_time).count();

        auto plain_data = this->_plain_data;

        auto start_upload_time = high_resolution_clock::now();
        protocol.upload(this->_plain_data, this->_lookup_value);
        auto end_upload_time = high_resolution_clock::now();
        auto upload_time = end_upload_time - start_upload_time;
        long upload_time_in_nanoseconds = duration_cast<nanoseconds>(upload_time).count();

        SecureReportQuery<DataType> query;
        query.initialize(this->_lookup_value, this->_is_match);
        unique_ptr<PlainQuery<DataType>> query_ptr(new SecureReportQuery<DataType>(query));
        protocol.query(move(query_ptr));

        auto start_retrieval_time = high_resolution_clock::now();
        auto result = protocol.retrieve();
        auto end_retrieval_time = high_resolution_clock::now();
        auto retrieval_time = end_retrieval_time - start_retrieval_time;
        long retrieval_time_in_nanoseconds = duration_cast<nanoseconds>(retrieval_time).count();

        // TODO Consider to Delete
        cout << "Result: " << VectorUtils::std_vector_to_string(result) << "\n";
         
         unique_ptr<Data> protocol_data_ptr(new SecureBatchRetrievalData<DataType>(this->_lookup_value, this->_database_size, this->_number_of_matches,
         this->_is_match, this->_plain_data, this->_data_type_ptr, initialize_time_in_nanoseconds, upload_time_in_nanoseconds, retrieval_time_in_nanoseconds,
         _batch_size, _batch_index));
        
        return protocol_data_ptr;
    }

private:
    int get_batch_size() {
        return _batch_size;
    }

    int get_batch_index() {
        return _batch_index;
    }
};


#endif //SECURE_SECURERETRIEVALTESTCASE_H
