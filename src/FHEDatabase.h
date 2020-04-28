#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SketchEncoder.h"
#include "VectorUtils.h"
#include "FHEUtils.h"
#include "Config.h"
#include "InputOutput.h"
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
 *      III. Add logger - [Done]
 *      IV. Move test flow outside this class
 * */

template<class Number>
class FHEDatabase {
private:
    int _d;
    int _n;
    PlainDatabase _database;
    const std::string _TABLE_NAME = "table_vector";
    InputOutput _io;
public:
    SketchEncoder _encoder;

public:
    FHEDatabase(int db_size, int sparsity):
            _n(db_size),
            _d(sparsity),
            _encoder(db_size, sparsity),
            _database(),
            _io(OUTPUT_TO_CONSOLE, OUTPUT_FILE_PATH, ERROR)
            {}

    // Test function
    bool test_sketch() {
        // Dummy database data
        VectorXi plain_data = VectorUtils::generate_binary_vector(_n, _d);

        // Dummy query value
        Number* lookup_value = new Number[1];
        lookup_value[0] = Number::static_from_int(1);

        // Save matched indices to indicate success/failure
        std::vector<int> indices = VectorUtils::getMatches(plain_data);

        // Connect to plain database
        bool database_connected = connect();
        if(!database_connected) {
            _io.output("Failed to connect to database\n", ERROR);
            return false;
        }

        // TODO build encrypted database - use encrypt_input method for it
        bool database_built = build_database_table(plain_data);
        if(!database_built) {
            _io.output("Failed to build database \n", ERROR);
            return false;
        }

        // Query database
        Number* matches_indicators = evaluate_matches_indicators(lookup_value, FHEUtils<Number>::areEqualBinary);

        // Encode encrypted data
        Number* encoded_encrypted_matches = fhe_encode(matches_indicators);

        // Decrypt encoded_encrypted_matches (which is encrypted encoding)
        VectorXi encoded_matches = fhe_decrypt(encoded_encrypted_matches);

        // Decode encoded into decoded
        VectorXi matches = decode(encoded_matches);

        // Print input and result
        _io.output("Input indices: \n", DEBUG);
        for (auto i = indices.begin(); i != indices.end(); ++i)
            std::cout << *i << ' ';
        _io.output("\n Decoded matches: \n", DEBUG);

        // Test if result is valid
        std::vector<int> decoded_std(matches.data(), matches.data() + matches.rows() * matches.cols());
        bool is_successful_decode = std::is_permutation(indices.begin(), indices.end(), decoded_std.begin());

        delete [] encoded_encrypted_matches;
        return is_successful_decode;
    }

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
            _io.output("Failed to construct database. Database values: " + VectorUtils::to_string(vector), ERROR);
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

    VectorXi fhe_decrypt(Number* encrypted_array) {
        /*
         * Description: decrypt encrypted vector
         * */
        MatrixXi sketch = _encoder.get_sketch();
        VectorXi encoded(sketch.rows());
        for (int i_output = 0; i_output < sketch.rows(); ++i_output) {
            encoded[i_output] = encrypted_array[i_output].to_int();
        }

        return encoded;
    }

    Number* evaluate_matches_indicators(Number* lookup_value, Number (*isMatch)(Number*, Number*, int)) {
        /*
         * Description: build matches vector based on database and query operator
         * */

        VectorXi database_row = _database.table_to_vector(_TABLE_NAME);
        Number* result = new Number[_n];

        for (int i = 0; i < database_row.size(); ++i) {
            /* TODO
             * 1. Once data is save as encrypted binary array, update isMatch activation
             * */

            Number* encrypted_element = new Number[1];
            encrypted_element[0] = Number::static_from_int(database_row(i));
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
};


#endif
