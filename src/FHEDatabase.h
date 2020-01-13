#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SketchEncoder.h"
#include "VectorUtils.h"
#include "FHEUtils.h"
#include <binomial_tournament.h>
#include <Ctxt.h>
#include <binaryCompare.h>

/* TODO
 * 1. Encrypt data before database init, such that database will be encrypted (currently database is saved plain)
 * 2. Expand database structure
 * 3. Expand test to int from binary values
 * 4. Code refactoring
 *  A. Better naming
 *  B. Order functions
 *  C. Improve design
 *      I. Improve debugger design
 *      II. Add option to create plain/encrypted/both database to debugger
 *      III. Add logger
 *      IV. Move test flow outside this class
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

    bool build_database_table(VectorXi vector) {
        /*
         * Description: Build database table with unique prime key and value columns
         * */
        /* TODO
         * 1. Should get Number* encrypted_input and init database with it
         * 2. Deiced how to store data -
         * Given simple database with int value (converted to binary array and encrypted) how are they represented in database
         * */

        bool table_constructed;
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

    Number* encrypt_input(VectorXi& plain_input) {
        Number* encrypted_input = new Number[_n];
        for (int i = 0; i < plain_input.size(); ++i) {
            encrypted_input[i] = Number::static_from_int(plain_input(i));
        }
        return encrypted_input;
    }

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

    Number* build_match_indices_vector(Number* lookup_value, Number (*isMatch)(Number*, Number*, int)) {
        /*
         * Description: build matches vector based on database and query operator
         * */

        VectorXi db_vector = _database.table_to_vector(_TABLE_NAME);
        Number* result = new Number[_n];

        for (int i = 0; i < db_vector.size(); ++i) {
            /* TODO
             * 1. Once data is save as encrypted binary array, update isMatch activation
             * */

            Number* encrypted_element = new Number[1];
            encrypted_element[0] = Number::static_from_int(db_vector(i));
            result[i] = isMatch(lookup_value, encrypted_element, 1);
        }
        return result;
    }

    inline Number* fhe_encode(Number* encrypted_matches_indicator) {
        /*
        * Description: Encode under full homomorphic encryption
        * */
        MatrixXi sketch = _encoder.get_sketch();
        Number* out = new Number[sketch.rows()];
        for (int i_out = 0; i_out < sketch.rows(); ++i_out)
            out[i_out] = Number::static_from_int(0);

        for (int i_input = 0; i_input < _n; ++i_input) {
            Number x = encrypted_matches_indicator[i_input];

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
        // Dummy database data
        VectorXi plain_vector = VectorUtils::generate_binary_vector(_n, _d);

        // Dummy query value
        Number* query_value = new Number[1];
        query_value[0] = Number::static_from_int(1);

        // Save matched indices to indicate success/failure
        std::vector<int> indices = VectorUtils::getMatches(plain_vector);

        // Connect to plain database
        bool database_connected = connect();
        if(!database_connected) {
            std::cerr << "Failed to connect to database" << std::endl;
            return false;
        }

        // TODO build encrypted database - use encrypt_input method for it
        bool database_built = build_database_table(plain_vector);
        if(!database_built) {
            std::cerr << "Failed to build database" << std::endl;
            return false;
        }

        // Query database
        Number* matches_vector = build_match_indices_vector(query_value, FHEUtils<Number>::areEqualBinary);

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
