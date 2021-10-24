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

// TODO examine file, add detailed TODO list, at top of the file

template<class DataType>
class SecureRetrievalTestCase: public TestCase {
protected:
    const string _name;
    int _lookup_value;
    int _database_size;
    vector<int> _matches_indices;
    int _number_of_matches_in_database;
    DataType (*_is_match)(DataType&, DataType&);
    vector<int> _plain_data;
    unique_ptr<DatabaseDataType> _data_type_ptr;

public:
    SecureRetrievalTestCase(const string& name, int lookup_value,int database_size, vector<int> matches_indices,
                        DataType (*is_match)(DataType&, DataType&), vector<int>& plain_data,
                        unique_ptr<DatabaseDataType>& data_type_ptr):
            _name{name},
            _lookup_value{lookup_value},
            _database_size{database_size}, _matches_indices{matches_indices}, _number_of_matches_in_database{int(matches_indices.size())},
            _is_match{is_match}, _plain_data{plain_data}, _data_type_ptr(move(data_type_ptr)) {}

    virtual string to_string() {
        string str = "_lookup_value = " + std::to_string(_lookup_value);
        str += "_database_size = " + std::to_string(_database_size);
        str += "_number_of_matches_in_database = " + std::to_string(_number_of_matches_in_database);
        return str;
    }

    virtual unique_ptr<Data> test() = 0;
protected:

    int get_lookup_value() {
        return _lookup_value;
    }

    int get_database_size() {
        return _database_size;
    }

    int get_number_of_matches_in_database() {
        return _number_of_matches_in_database;
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
class SecureReportTestCase: public SecureRetrievalTestCase<DataType> {
public:
    SecureReportTestCase(const string& name, int lookup_value,int database_size, vector<int> matches_indices,
                            DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                            unique_ptr<DatabaseDataType>& data_type_ptr): SecureRetrievalTestCase<DataType>(name, lookup_value, database_size, matches_indices,
                                          isMatch, plain_data, data_type_ptr) {}

    unique_ptr<Data> test() override {
        SecureReportClient<DataType> client(this->_data_type_ptr);
        SecureReportServer<DataType> server(this->_database_size, this->_number_of_matches_in_database);
        SecureReportProtocol<DataType> protocol(server, client);

        auto start_initialize_time = high_resolution_clock::now();
        protocol.initialize();
        auto end_initialize_time = high_resolution_clock::now();
        auto initialize_time = end_initialize_time - start_initialize_time;
        long initialize_time_in_nanoseconds = duration_cast<nanoseconds>(initialize_time).count();

        auto plain_data = this->_plain_data;

        auto start_upload_time = high_resolution_clock::now();
        protocol.upload(this->_plain_data);
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


        // auto data = SecureRetrievalData<DataType>(this->_name, this->_lookup_value, this->_database_size, this->_matches_indices, result,
        //  this->_is_match, this->_plain_data, this->_data_type_ptr,initialize_time_in_nanoseconds,
        //  upload_time_in_nanoseconds, retrieval_time_in_nanoseconds);
        int number_of_matches_in_database = this->_matches_indices.size();
        unique_ptr<SecureRetrievalData<DataType>> protocol_data_ptr = make_unique<SecureRetrievalData<DataType>>(
            this->_name, this->_lookup_value, this->_database_size, number_of_matches_in_database,
            this->_matches_indices, result,this->_is_match, this->_plain_data, this->_data_type_ptr, 
            initialize_time_in_nanoseconds, upload_time_in_nanoseconds, retrieval_time_in_nanoseconds);

        // unique_ptr<RetrievalTestResult<int>> test_result = make_unique<RetrievalTestResult<int>>(RetrievalTestResult<int>(selected_matches_indices, evaluated_matches_indices, database_size));

        return protocol_data_ptr;
    }
};

template<class DataType>
class SecureBatchRetrievalTestCase: public SecureRetrievalTestCase<DataType> {
    int _batch_size;
    int _requested_number_of_matches;

public:
    SecureBatchRetrievalTestCase(const string& name, int lookup_value,int database_size, int requested_number_of_matches, vector<int>  matches_indices,
                            DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
                            unique_ptr<DatabaseDataType>& data_type_ptr, int batch_size):
            _batch_size{batch_size}, _requested_number_of_matches{requested_number_of_matches},
             SecureRetrievalTestCase<DataType>(name, lookup_value, database_size, matches_indices,
                                          isMatch, plain_data, data_type_ptr) {}

    string to_string()  override{
        string str = SecureRetrievalTestCase<DataType>::to_string();
        str += "batch_size = " + std::to_string(get_batch_size());
        str += "requested_number_of_matches = " + std::to_string(_requested_number_of_matches);
        str += "number_of_batches = " + std::to_string(get_number_of_batches());
        return str;
    }

    unique_ptr<Data> test() override {
        SecureBatchRetrievalClient<DataType> client(this->_data_type_ptr);
        SecureBatchRetrievalServer<DataType> server(this->_database_size, this->_number_of_matches_in_database);
        SecureBatchRetrievalProtocol<DataType> protocol(server, client);

        auto start_initialize_time = high_resolution_clock::now();
        protocol.initialize();
        auto end_initialize_time = high_resolution_clock::now();
        auto initialize_time = end_initialize_time - start_initialize_time;
        long initialize_time_in_nanoseconds = duration_cast<nanoseconds>(initialize_time).count();

        auto plain_data = this->_plain_data;

        auto start_upload_time = high_resolution_clock::now();
        protocol.upload(this->_plain_data);
        auto end_upload_time = high_resolution_clock::now();
        auto upload_time = end_upload_time - start_upload_time;
        long upload_time_in_nanoseconds = duration_cast<nanoseconds>(upload_time).count();

        auto start_retrieval_time = high_resolution_clock::now();
        auto result = retrieve_all_batches(protocol);
        auto end_retrieval_time = high_resolution_clock::now();
        auto retrieval_time = end_retrieval_time - start_retrieval_time;
        long retrieval_time_in_nanoseconds = duration_cast<nanoseconds>(retrieval_time).count();


        vector<int> evaluated_matches_indices;
        for(auto && v : result){
            evaluated_matches_indices.insert(evaluated_matches_indices.end(), v.begin(), v.end());
        }
         
         int number_of_matches_in_database = this->_matches_indices.size();
         unique_ptr<SecureBatchRetrievalData<DataType>> protocol_data_ptr = make_unique<SecureBatchRetrievalData<DataType>>(
            this->_name, this->_lookup_value, this->_database_size, number_of_matches_in_database, _requested_number_of_matches, this->_matches_indices, evaluated_matches_indices,
            this->_is_match, this->_plain_data, this->_data_type_ptr, upload_time_in_nanoseconds, initialize_time_in_nanoseconds, retrieval_time_in_nanoseconds, _batch_size);
        
        return protocol_data_ptr;

        // name, int lookup_value,int database_size, int number_of_matches_in_database, int number_of_requested_matches, vector<int> selected_matches_indices, vector<int> evaluated_matches_indices,
        //                 DataType (*isMatch)(DataType&, DataType&), vector<int> plain_data,
        //                 unique_ptr<DatabaseDataType>& data_type_ptr, long upload_time, long initialize_time, long retrieval_time, 
        //                 int batch_size

        
        // unique_ptr<SecureRetrievalData<DataType>> protocol_data_ptr = make_unique<SecureRetrievalData<DataType>>(
        //     this->_name, this->_lookup_value, this->_database_size, number_of_matches_in_database,
        //     this->_matches_indices, result,this->_is_match, this->_plain_data, this->_data_type_ptr, 
        //     initialize_time_in_nanoseconds, upload_time_in_nanoseconds, retrieval_time_in_nanoseconds);
    }

private:
    vector<vector<int>> retrieve_all_batches(SecureBatchRetrievalProtocol<DataType>& protocol) {
        vector<vector<int>> batches_accumulator;
        auto number_of_required_batches = get_number_of_required_batches();
        
        for(int i = 0; i < number_of_required_batches; ++i) {
            SecureBatchRetrievalQuery<DataType> query;
            query.initialize(this->_lookup_value, _batch_size, i, this->_database_size, this->_number_of_matches_in_database, this->_is_match);
            unique_ptr<PlainQuery<DataType>> query_ptr(new SecureBatchRetrievalQuery<DataType>(query));
            protocol.query(move(query_ptr));
            auto batch = protocol.retrieve();
            batches_accumulator.push_back(batch);
        }
        return batches_accumulator;
    }

    int get_batch_size() {
        return _batch_size;
    }

    int get_number_of_required_batches() {
        // TODO: After implement approximate counting, use Approximate counting to implement this function
        int number_of_required_batches = int(ceil(_requested_number_of_matches / _batch_size));
        return number_of_required_batches;
    }

    int get_number_of_batches() {
        // TODO: After implement approximate counting, use Approximate counting to implement this function
        int number_of_batches = int(ceil(this->_database_size / _batch_size));
        return number_of_batches;
    }
};


#endif //SECURE_SECURERETRIEVALTESTCASE_H
