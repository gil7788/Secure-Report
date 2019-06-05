#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SketchEncoder.h"
#include "VectorUtils.h"

/*
 * TODO
 * 1. Split test flow into functions - Done
 * 2. Add real queries - read and understand max's email about fhe_is_equal function
 * 3. Integrate real db - Done
 * 4. Build db that contains costume type of objects (i.e. structs, classes) -
 *      I. Current solution, build functions and api that supports students needs (include enhancing Plaindatabase class)
 *      II. Ideal solution - Integrate SQL parser to the system, such that FHEDatabase will support a
 *      subset of SQL queries, and then parser is not needed
 *
 * */

template<class Number>
class FHEDatabase {
private:
    int _d;
    int _n;
    PlainDatabase _database;
    const std::string _TABLE_NAME = "table_vector";
public:
    SketchEncoder _encoder;

public:
    FHEDatabase(int db_size, int sparsity):
            _n(db_size),
            _d(sparsity),
            _encoder(db_size, sparsity),
            _database()
            {}

    // Database related functions
    bool connect() {
        /*
         * Description: Connect to database
         * */
        bool database_connected = _database.connect();
        return database_connected;
    }

    bool build_database_table(VectorXi& vector) {
        /*
         * Description: Build database table with unique prime key and value columns
         * */
        /*TODO
         * 1. Add costume types build of database
         * 2. Add encrypted build
         * */
        bool table_constructed = false;
        if(_database.table_exists(_TABLE_NAME)) {
            table_constructed =_database.update_table(_TABLE_NAME, vector);
        }
        else {
            table_constructed = _database.initiate_table(_TABLE_NAME, vector);
        }

        if(!table_constructed) {
            if(DEBUG_LEVEL >= 2) {
                string str_vector = VectorUtils::to_string(vector);
                cerr << str_vector << endl;
            }
            return false;
        }
        return true;
    }

    // Encryption/encoding related functions
    VectorXi decode(VectorXi encoded) {
        /*
         * Description: Decode plain vector
         * */
        for (int i = 0; i < encoded.size(); ++i) {
            (encoded[i] > 0) ? encoded[i] = 1 : encoded[i] = 0;
        }
        VectorXi decoded = _encoder.decode(encoded);
        return decoded;
    }

    VectorXi fhe_decrypt(Number* sketchEnc) {
        /*
         * Description: decrypt encrypted vector
         * */
        MatrixXi sketch = _encoder.get_sketch();
        VectorXi encoded(sketch.rows());
        for (int i_output = 0; i_output < sketch.rows(); ++i_output) {
            encoded[i_output] = sketchEnc[i_output].to_int();
        }

        return encoded;
    }

    VectorXi build_match_indices_vector(Number value) {
        /*
         * Description: build matches vector based on database and query operator
         * */
        /*TODO
         * Currently this function is a dummy implementation.
         * 1. Implement fhe equality
         * 2. Implement few or one generic function that can implement few comparison types, i.e. eq, leq, less, geq, greater
         * */
        VectorXi db_vector = _database.table_to_vector(_TABLE_NAME);
        VectorXi result(db_vector.size());
        for (int i = 0; i < db_vector.size(); ++i) {
            // TODO TEST equality
            result(i) = db_vector(i);
        }
        return result;
    }

    inline Number* fhe_encode(VectorXi plain_vector) {
        /*
        * Description: Encode under full homomorphic encryption
        * */
        /*TODO
         * 1. Change implementation, currently the database values are saved plain and encrypted when fhe_encode function access them.
         * Desired behavior: add selection to to save values encrypted/plain, if selected decision is to save them encrypted,
         * then save them in encrypted database
         * */
        MatrixXi sketch = _encoder.get_sketch();
        Number* out = new Number[sketch.rows()];
        for (int i_out = 0; i_out < sketch.rows(); ++i_out)
            out[i_out] = Number::static_from_int(0);

        for (int i_input = 0; i_input < plain_vector.size(); ++i_input) {
            Number x = Number::static_from_int(plain_vector[i_input]);

            for (int i_out = 0; i_out < sketch.rows(); ++i_out) {
                if (sketch(i_out, i_input) == 1)
                    out[i_out] += x;
            }
        }
        return out;
    }

// Test functions
public:
    bool test_sketch() {
        // Dummy data
        // Dummy database data
        VectorXi plain_vector = VectorUtils::generate_binary_vector(_n, _d);
        // Dummy query value
        Number query_value = Number::static_from_int(0);

        // Save matched indices for to indicate success/failure
        std::vector<int> indices = VectorUtils::getMatches(plain_vector);

        // Connect to plain database
        bool database_connected = connect();
        if(!database_connected) {
            std::cerr << "Failed to connect to database" << std::endl;
            return false;
        }

        // Build database table
        bool database_built = build_database_table(plain_vector);
        if(!database_built) {
            std::cerr << "Failed to build database" << std::endl;
            return false;
        }

        // Query database
        VectorXi matches_vector = build_match_indices_vector(query_value);

        // Encode encrypted data
        Number* sketchEnc = fhe_encode(matches_vector);

        // Decrypt sketchEnc (which is encrypted encoding)
        VectorXi encoded = fhe_decrypt(sketchEnc);

        // Decode encoded into decoded
        VectorXi decoded = decode(encoded);

        // Print input and result
        std::cout << "Input indices: " << std::endl;
        for (auto i = indices.begin(); i != indices.end(); ++i)
            std::cout << *i << ' ';
        std::cout << std::endl;
        std::cout << "Decoded: " << decoded << std::endl;

        // Test if result is valid
        std::vector<int> decoded_std(decoded.data(), decoded.data() + decoded.rows() * decoded.cols());
        bool is_successful_decode = std::is_permutation(indices.begin(), indices.end(), decoded_std.begin());

        delete [] sketchEnc;
        return is_successful_decode;
    }
};


#endif
