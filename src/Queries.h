//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_QUERIES_H
#define SECURE_REPORT_QUERIES_H

#include "vector"


using namespace std;

template <typename DataType>
class EncryptedQuery {
public:
    virtual ~EncryptedQuery() = default;
};

template <typename DataType>
class PlainQuery {
public:
//    virtual EncryptedQuery<DataType> encrypt() = 0;
};

template <typename DataType>
class EncryptedSecureReportQuery: public EncryptedQuery<DataType> {
public:
    DataType _encrypted_lookup_value;
    DataType (*_isMatch)(DataType&, DataType&);

    void initialize(int lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        initialize(DataType(lookup_value), isMatch);
    }

    void initialize(DataType encrypted_lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _isMatch = isMatch;
    }
};

template <typename DataType>
class SecureReportQuery: public PlainQuery<DataType> {
private:
    int _lookup_value;
    DataType (*_isMatch)(DataType&, DataType&);

public:
    void initialize(int lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        _lookup_value = lookup_value;
        _isMatch = isMatch;
    }

    EncryptedSecureReportQuery<DataType> encrypt() {
        EncryptedSecureReportQuery<DataType> result;
        result.initialize(_lookup_value, _isMatch);
        return result;
    }
};

template <typename DataType>
class EncryptedSecureBatchRetrievalQuery: public EncryptedQuery<DataType> {
public:
    DataType _encrypted_lookup_value;
    int _batch_size;
    int _batch_index;
    DataType (*_isMatch)(DataType&, DataType&);

    void initialize(int lookup_value, int batch_index, int batch_size,
                    DataType (*isMatch)(DataType&, DataType&)) {
        initialize(DataType(lookup_value), batch_index, batch_size, isMatch);
    }

    void initialize(DataType encrypted_lookup_value, int batch_index, int batch_size,
                                       DataType (*isMatch)(DataType&, DataType&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _batch_index = batch_index;
        _batch_size = batch_size;
        _isMatch = isMatch;
    }
};

template <typename DataType>
class SecureBatchRetrievalQuery: public PlainQuery<DataType> {
public:
    int _lookup_value;
    int _batch_size;
    int _batch_index;
    int _database_size;
    int _number_of_matches;
    DataType (*_isMatch)(DataType&, DataType&);

public:
    void initialize(int lookup_value, int batch_size, int batch_index, int database_size, int number_of_matches, DataType (*isMatch)(DataType&, DataType&)) {
        _lookup_value = lookup_value;
        _batch_size = batch_size;
        _batch_index = batch_index;
        _database_size = database_size;
        _number_of_matches = number_of_matches;
        _isMatch = isMatch;
    }

    EncryptedSecureBatchRetrievalQuery<DataType> encrypt() {
        EncryptedSecureBatchRetrievalQuery<DataType> result;
        result.initialize(_lookup_value, _batch_index, _batch_size, _isMatch);
        return result;
    }
};

#endif //SECURE_REPORT_QUERIES_H
