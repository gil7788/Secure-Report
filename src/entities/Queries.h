//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_QUERIES_H
#define SECURE_REPORT_QUERIES_H

#include "vector"


using namespace std;
/**
 * Encrypted query virtual class
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class EncryptedQuery {
public:
    virtual ~EncryptedQuery() = default;
};

/**
 * Plain query virtual class @see FHEDatabaseConfig.h
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class PlainQuery {
public:
    /**
     * Encrypt plain data in PlainQuery
     * @return unique_ptr<EncryptedQuery<DataType>> of encrypted data
     */
    virtual unique_ptr<EncryptedQuery<DataType>> encrypt() = 0;
};

/**
 * Secure Report encrypted query class
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class EncryptedSecureReportQuery: public EncryptedQuery<DataType> {
public:
    /**
     * Encrypted lookup value
     */
    DataType _encrypted_lookup_value;
    /**
     * Is match comperator @see FHEUtils.h
     */
    DataType (*_isMatch)(DataType&, DataType&);

    /**
     * Initialize query with integer number and IsMatch comparator (see FHEUtils.h)
     * @param lookup_value Look up value
     * @param isMatch see FHEUtils.h
     */
    void initialize(int lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        auto encrypted_look_up_value = DataType(lookup_value);
        initialize(encrypted_look_up_value, isMatch);
    }

    /**
     * Initialize query with Fully Homomorphic number (see DatabaseDataType)
     * and IsMatch comparator (see FHEUtils.h)
     * @param lookup_value See DatabaseDataType
     * @param isMatch See FHEUtils.h
     */
    void initialize(DataType& encrypted_lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _isMatch = isMatch;
    }
};

/**
 * Secure Report query class
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class SecureReportQuery: public PlainQuery<DataType> {
private:
    int _lookup_value;
    DataType (*_isMatch)(DataType&, DataType&);

public:
    /**
     * Initialize query with integer number and IsMatch comparator (see FHEUtils.h)
     * @param lookup_value Integer look up value
     * @param isMatch see FHEUtils.h
     */
    void initialize(int lookup_value, DataType (*isMatch)(DataType&, DataType&)) {
        _lookup_value = lookup_value;
        _isMatch = isMatch;
    }
    /**
     * Encrypt lookup value
     * @return EncryptedSecureReportQuery of encrypted lookup value
     */
    unique_ptr<EncryptedQuery<DataType>> encrypt() {
        unique_ptr<EncryptedSecureReportQuery<DataType>> result(new EncryptedSecureReportQuery<DataType>);
        result.get()->initialize(_lookup_value, _isMatch);
        return result;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class EncryptedSecureBatchRetrievalQuery: public EncryptedQuery<DataType> {
public:
//    Encrypted lookup value
    DataType _encrypted_lookup_value;

//    Batch size to fetch from server
    int _batch_size;

//    Batch index to fetch from server
    int _batch_index;

// IsMatch comparator, see FHEUtils
    DataType (*_isMatch)(DataType&, DataType&);

    /**
     * Initialize query with integer number, batch index, batch size, and IsMatch comparator (see FHEUtils.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param isMatch see FHEUtils.h
     */
    void initialize(int lookup_value, int batch_index, int batch_size,
                    DataType (*isMatch)(DataType&, DataType&)) {
        initialize(DataType(lookup_value), batch_index, batch_size, isMatch);
    }

    /**
     * Initialize query with Fully Homomorphic number (see DatabaseDataType),
     * batch index, batch size, and IsMatch comparator (see FHEUtils.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param isMatch see FHEUtils.h
     */
    void initialize(DataType encrypted_lookup_value, int batch_index, int batch_size,
                                       DataType (*isMatch)(DataType&, DataType&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _batch_index = batch_index;
        _batch_size = batch_size;
        _isMatch = isMatch;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam DataType Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from DatabaseDataType.
 */
template <typename DataType>
class SecureBatchRetrievalQuery: public PlainQuery<DataType> {
public:
//    Look up value
    int _lookup_value;

//    Batch size to fetch from server
    int _batch_size;

//    Batch index to fetch from server
    int _batch_index;

//    Database size
    int _database_size;

//    Number of matches in database
    int _number_of_matches;

//    IsMatch comparator, see FHEUtils
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
/**
 * Encrypt SecureBatchRetrievalQuery to EncryptedSecureBatchRetrievalQuery
 * @return unique_ptr<EncryptedQuery<DataType>> of encrypted look up value
 */
    unique_ptr<EncryptedQuery<DataType>> encrypt() {
        unique_ptr<EncryptedSecureBatchRetrievalQuery<DataType>> result(new EncryptedSecureBatchRetrievalQuery<DataType>);
        result.get()->initialize(_lookup_value, _batch_index, _batch_size, _isMatch);
        return result;
    }
};

#endif //SECURE_REPORT_QUERIES_H
