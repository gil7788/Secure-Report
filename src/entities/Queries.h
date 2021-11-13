//
// Created by gil on 26.8.2020.
//

#ifndef SECURE_REPORT_QUERIES_H
#define SECURE_REPORT_QUERIES_H

#include "vector"


using namespace std;
/**
 * Encrypted query virtual class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class EncryptedQuery {
public:
    virtual ~EncryptedQuery() = default;
};

/**
 * Plain query virtual class @see FHEDatabaseConfig.h
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class PlainQuery {
public:
    /**
     * Encrypt plain data in PlainQuery
     * @return unique_ptr<EncryptedQuery<Number>> of encrypted data
     */
    virtual unique_ptr<EncryptedQuery<Number>> encrypt() = 0;
};

/**
 * Secure Report encrypted query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class EncryptedSecureReportQuery: public EncryptedQuery<Number> {
public:
    /**
     * Encrypted lookup value
     */
    Number _encrypted_lookup_value;
    /**
     * Is match comperator @see Comparators.h
     */
    Number (*_is_match)(Number&, Number&);

    /**
     * Initialize query with integer number and IsMatch comparator (see Comparators.h)
     * @param lookup_value Look up value
     * @param is_match see Comparators.h
     */
    void initialize(int lookup_value, Number (*_is_match)(Number&, Number&)) {
        auto encrypted_look_up_value = Number(lookup_value);
        initialize(encrypted_look_up_value, _is_match);
    }

    /**
     * Initialize query with Fully Homomorphic number (see VirtualContext)
     * and IsMatch comparator (see Comparators.h)
     * @param lookup_value See VirtualContext
     * @param is_match See Comparators.h
     */
    void initialize(Number& encrypted_lookup_value, Number (*is_match)(Number&, Number&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _is_match = is_match;
    }
};

/**
 * Secure Report query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class SecureReportQuery: public PlainQuery<Number> {
private:
    int _lookup_value;
    Number (*_is_match)(Number&, Number&);

public:
    /**
     * Initialize query with integer number and IsMatch comparator (see Comparators.h)
     * @param lookup_value Integer look up value
     * @param is_match see Comparators.h
     */
    void initialize(int lookup_value, Number (*is_match)(Number&, Number&)) {
        _lookup_value = lookup_value;
        _is_match = is_match;
    }
    /**
     * Encrypt lookup value
     * @return EncryptedSecureReportQuery of encrypted lookup value
     */
    unique_ptr<EncryptedQuery<Number>> encrypt() {
        unique_ptr<EncryptedSecureReportQuery<Number>> result(new EncryptedSecureReportQuery<Number>);
        result.get()->initialize(_lookup_value, _is_match);
        return result;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class EncryptedSingleBatchRetrievalQuery: public EncryptedQuery<Number> {
public:
//    Encrypted lookup value
    Number _encrypted_lookup_value;

//    Batch size to fetch from server
    int _batch_size;

//    Batch index to fetch from server
    int _batch_index;

// IsMatch comparator, see Comparators
    Number (*_is_match)(Number&, Number&);

    /**
     * Initialize query with integer number, batch index, batch size, and IsMatch comparator (see Comparators.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param is_match see Comparators.h
     */
    void initialize(int lookup_value, int batch_index, int batch_size,
                    Number (*_is_match)(Number&, Number&)) {
        initialize(Number(lookup_value), batch_index, batch_size, _is_match);
    }

    /**
     * Initialize query with Fully Homomorphic number (see VirtualContext),
     * batch index, batch size, and IsMatch comparator (see Comparators.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param is_match see Comparators.h
     */
    void initialize(Number encrypted_lookup_value, int batch_index, int batch_size,
                                       Number (*is_match)(Number&, Number&)) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _batch_index = batch_index;
        _batch_size = batch_size;
        _is_match = is_match;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class SingleBatchRetrievalQuery: public PlainQuery<Number> {
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

//    IsMatch comparator, see Comparators
    Number (*_is_match)(Number&, Number&);

public:
    void initialize(int lookup_value, int batch_size, int batch_index, int database_size, int number_of_matches, Number (*is_match)(Number&, Number&)) {
        _lookup_value = lookup_value;
        _batch_size = batch_size;
        _batch_index = batch_index;
        _database_size = database_size;
        _number_of_matches = number_of_matches;
        _is_match = is_match;
    }
/**
 * Encrypt SingleBatchRetrievalQuery to EncryptedSingleBatchRetrievalQuery
 * @return unique_ptr<EncryptedQuery<Number>> of encrypted look up value
 */
    unique_ptr<EncryptedQuery<Number>> encrypt() {
        unique_ptr<EncryptedSingleBatchRetrievalQuery<Number>> encrypted_query(new EncryptedSingleBatchRetrievalQuery<Number>);
        encrypted_query.get()->initialize(_lookup_value, _batch_index, _batch_size, _is_match);
        return encrypted_query;
    }

    EncryptedSingleBatchRetrievalQuery<Number> encrypt_single_batch() {
        EncryptedSingleBatchRetrievalQuery<Number> encrypted_query;
        encrypted_query.initialize(_lookup_value, _batch_index, _batch_size, _is_match);
        return encrypted_query;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class EncryptedMultipleBatchRetrievalQuery: public EncryptedQuery<Number> {
private:
    vector<EncryptedSingleBatchRetrievalQuery<Number>> _encrypted_single_batch_queries;
    int _batch_query_index = 0;

//    Number of batches to fetch from server
    int _number_of_batches;
public:
//    Encrypted lookup value
    Number _encrypted_lookup_value;

//    Batch size to fetch from server
    int _batch_size;

// IsMatch comparator, see Comparators
    Number (*_is_match)(Number&, Number&);

    /**
     * Initialize query with integer number, batch index, batch size, and IsMatch comparator (see Comparators.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param is_match see Comparators.h
     */

    void initialize(int lookup_value, int number_of_batches, int batch_size,
                    Number (*is_match)(Number&, Number&), vector<EncryptedSingleBatchRetrievalQuery<Number>> & encrypted_single_batch_queries) {
        initialize(Number(lookup_value), number_of_batches, batch_size, is_match, encrypted_single_batch_queries);
    }

    /**
     * Initialize query with Fully Homomorphic number (see VirtualContext),
     * batch index, batch size, and IsMatch comparator (see Comparators.h)
     * @param lookup_value Integer look up value
     * @param batch_index Batch index to fetch from server
     * @param batch_size Batch size to fetch from server
     * @param is_match see Comparators.h
     */
    void initialize(Number encrypted_lookup_value, int number_of_batches, int batch_size,
                    Number (*is_match)(Number&, Number&), vector<EncryptedSingleBatchRetrievalQuery<Number>>& encrypted_single_batch_queries) {
        _encrypted_lookup_value = encrypted_lookup_value;
        _number_of_batches = number_of_batches;
        _batch_size = batch_size;
        _is_match = is_match;
        _encrypted_single_batch_queries = encrypted_single_batch_queries;
    }

    int get_number_of_batches() {
        return _encrypted_single_batch_queries.size();
    }

    EncryptedSingleBatchRetrievalQuery<Number> get_next_batch_query() {
//          TODO Add exception once all queries/batch are retrieved
        if(_encrypted_single_batch_queries.size() == _batch_query_index) {
            cout << "Error: Access non existing batch query" << endl;
        }

        auto batch_query = _encrypted_single_batch_queries[_batch_query_index];
        _batch_query_index += 1;
        return batch_query;
    }
};

/**
 * Encrypted Secure Batch Retrieval query class
 * @tparam Number Plain/Encrypted Fully Homomorphic number template,
 * plausible templates inherit from VirtualContext.
 */
template <typename Number>
class MultipleBatchRetrievalQuery: public PlainQuery<Number> {
private:
    vector<SingleBatchRetrievalQuery<Number>> _single_batch_queries;
    int _batch_query_index = 0;

public:
//    Look up value
    int _lookup_value;

//    Batch size to fetch from server
    int _batch_size;

//    Number of batches to fetch from server
    int _number_of_batches;

//    Database size
    int _database_size;

//    Number of matches in database
    int _number_of_matches;

//    IsMatch comparator, see Comparators
    Number (*_is_match)(Number&, Number&);

public:
    void initialize(int lookup_value, int batch_size, int number_of_batches, int database_size, int number_of_matches, Number (*is_match)(Number&, Number&)) {
        _lookup_value = lookup_value;
        _batch_size = batch_size;
        _number_of_batches = number_of_batches;
        _database_size = database_size;
        _number_of_matches = number_of_matches;
        _is_match = is_match;
        initialize_single_batch_queries();
    }
/**
 * Encrypt SingleBatchRetrievalQuery to EncryptedSingleBatchRetrievalQuery
 * @return unique_ptr<EncryptedQuery<Number>> of encrypted look up value
 */
    unique_ptr<EncryptedQuery<Number>> encrypt() {
        auto encrypted_single_batch_queries = encrypt_single_batch_queries();

        unique_ptr<EncryptedMultipleBatchRetrievalQuery<Number>> encrypted_query(new EncryptedMultipleBatchRetrievalQuery<Number>);
        encrypted_query.get()->initialize(_lookup_value, _number_of_matches, _batch_size, _is_match, encrypted_single_batch_queries);
        return encrypted_query;
    }

    int get_number_of_batches() {
        return _single_batch_queries.size();
    }

    SingleBatchRetrievalQuery<Number> get_next_batch_query() {
//          TODO Add exception once all queries/batch are retrieved
        if(_single_batch_queries.size() == _batch_query_index) {
            cout << "Error: Access non existing batch query" << endl;
        }

        auto batch_query = _single_batch_queries[_batch_query_index];
        _batch_query_index += 1;
        return batch_query;
    }

private:
    void initialize_single_batch_queries() {
        for (int i = 0; i < _number_of_batches; ++i) {
            SingleBatchRetrievalQuery<Number> query;
            query.initialize(_lookup_value, _batch_size, i, _database_size, _number_of_matches, _is_match);
            _single_batch_queries.push_back(query);
        }
    }

    vector<EncryptedSingleBatchRetrievalQuery<Number>> encrypt_single_batch_queries() {
        vector<EncryptedSingleBatchRetrievalQuery<Number>> encrypted_single_batch_queries;
        for(auto &query: _single_batch_queries) {
            EncryptedSingleBatchRetrievalQuery<Number> encrypted_query = query.encrypt_single_batch();
            encrypted_single_batch_queries.push_back(encrypted_query);
        }
        return encrypted_single_batch_queries;
    }
};

#endif //SECURE_REPORT_QUERIES_H
