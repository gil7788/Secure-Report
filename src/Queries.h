//
// Created by gil on 26.8.2020.
//

#include "vector"
#ifndef SECURE_REPORT_QUERIES_H
#define SECURE_REPORT_QUERIES_H

using namespace std;

template <typename DataType>
class EncryptedSecureReportQuery {
public:
    DataType _encrypted_lookup_value;
    DataType (*_isMatch)(DataType&, DataType&);

    EncryptedSecureReportQuery(DataType encrypted_lookup_value, DataType (*isMatch)(DataType&, DataType&)):
            _encrypted_lookup_value(encrypted_lookup_value),
            _isMatch{isMatch} {}

    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database) {
        vector<DataType> isMatch_indicator;
        for(auto& element: database){
            auto element_isMatch_indicator = evaluate_is_match_on_element(element);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType element) {
        return _isMatch(_encrypted_lookup_value, element);
    }
};

template <typename DataType>
class SecureReportQuery {
private:
    int _lookup_value;
    DataType (*_isMatch)(DataType&, DataType&);

public:
    SecureReportQuery(int lookup_value, DataType (*isMatch)(DataType&, DataType&)):
            _lookup_value{lookup_value},
            _isMatch{isMatch} {}

    int get_lookup_value() {
        return _lookup_value;
    }

    EncryptedSecureReportQuery<DataType> encrypt() {
        DataType encrypted_lookup_value = DataType(_lookup_value);
        return EncryptedSecureReportQuery<DataType>{encrypted_lookup_value, _isMatch};
    }
};

template <typename DataType>
class EncryptedSecureBatchRetrievalQuery {
public:
    DataType _encrypted_lookup_value;
    vector<int> _batches_split;
    int _batch_index;
    DataType (*_isMatch)(DataType&, DataType&);

    EncryptedSecureBatchRetrievalQuery(DataType& encrypted_lookup_value, vector<int>& batches_split, int batch_index,
                                       DataType (*isMatch)(DataType&, DataType&)):
            _encrypted_lookup_value(encrypted_lookup_value),
            _batches_split{batches_split},
            _batch_index{batch_index},
            _isMatch{isMatch} {}

    vector<DataType> evaluate_is_match_on_database(vector<DataType>& database) {
        vector<DataType> isMatch_indicator;
        for(int element_index = 0; element_index <  database.size(); ++element_index) {
            auto element_isMatch_indicator = evaluate_is_match_on_element(database[element_index], element_index);
            isMatch_indicator.push_back(element_isMatch_indicator);
        }
        return isMatch_indicator;
    }

    DataType evaluate_is_match_on_element(DataType element, int element_index) {
        if(_batches_split[element_index] == _batch_index) {
            return _isMatch(_encrypted_lookup_value, element);
        }
        else {
            return DataType(0);
        }
    }
};

template <typename DataType>
class SecureBatchRetrievalQuery {
private:
    int _lookup_value;
    int _batch_size;
    int _batch_index;
    int _database_size;
    DataType (*_isMatch)(DataType&, DataType&);

public:
    SecureBatchRetrievalQuery(int lookup_value, int batch_size, int batch_index, int database_size, DataType (*isMatch)(DataType&, DataType&)):
            _lookup_value{lookup_value},
            _batch_size{batch_size},
            _batch_index{batch_index},
            _database_size{database_size},
            _isMatch{isMatch} {}

    EncryptedSecureBatchRetrievalQuery<DataType> encrypt() {
        DataType encrypted_lookup_value = DataType(_lookup_value);
        auto batches_split = split_database_to_batches();
        return EncryptedSecureBatchRetrievalQuery<DataType>{encrypted_lookup_value, batches_split, _batch_index, _isMatch};
    }

    // TODO: re-implement dummy
    vector<int> split_database_to_batches() {
        vector<int> vec(_database_size/2, 0);
        for (int i = 0; i < _database_size/2; ++i) {
            vec.push_back(1);
        }
        return vec;
    }
};

#endif //SECURE_REPORT_QUERIES_H
