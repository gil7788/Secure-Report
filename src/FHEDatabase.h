#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SketchEncoder.h"
#include "VectorUtils.h"
#include "FHEUtils.h"
#include "Config.h"
#include "InputOutput.h"
#include "Client.h"
#include "TrustedThirdParty.h"
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
 *      II. Add option to create plain/encrypted/both database to debugger - [Done partially] implemented plain and encrypted
 *      III. Add logger - [Done]
 *      IV. Move test flow outside this class - [Done]
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
    FHEDatabase(int db_size, int sparsity, InputOutput& io):
            _n(db_size),
            _d(sparsity),
            _database(),
            _io(io)
            {}

    // Server related functions
    bool connect() {
        /*
         * Description: Connect to database
         * */
        bool database_connected = _database.connect();
        return database_connected;
    }

    bool build_database_table(std::vector<int>& vector) {
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
            _io.output("Failed to construct database. Server values: " + VectorUtils::std_vector_to_string(vector), constants::OUTPUT_LEVELS::ERROR);
            return false;
        }
        return true;
    }

    std::vector<Number> evaluate_matches_indicators(EncryptedSecureReportQuery<Number>& encrypted_query) {
        /*
         * Description: build matches vector based on database and query operator
         * */

        VectorXi database_row = _database.table_to_vector(_TABLE_NAME);

        std::vector<Number> result(_n);

        for (int i = 0; i < database_row.size(); ++i) {
            /* TODO
             * 1. Once data is save as encrypted binary array, update isMatch activation
             * */

            Number encrypted_element{database_row(i)};
            result[i] = encrypted_query._isMatch(encrypted_query._encrypted_lookup_value, encrypted_element, 1);
        }
        return result;
    }

    inline std::vector<Number> fhe_encode(std::vector<Number>& encrypted_matches_indicator,
                                          TrustedThirdParty& trusted_third_party) {
        /*
        * Description: Encode under full homomorphic encryption
        * */
        MatrixXi sketch = get_sketch(trusted_third_party);
        std::vector<Number> out(sketch.rows(), Number::static_from_int(0));

        for (int i_input = 0; i_input < _n; ++i_input) {
            Number x = encrypted_matches_indicator[i_input];

            for (int i_out = 0; i_out < sketch.rows(); ++i_out) {
                if (sketch(i_out, i_input) == 1)
                    out[i_out] += x;
            }
        }
        return out;
    }

    MatrixXi get_sketch(TrustedThirdParty& trusted_third_party) {
        return trusted_third_party._encoder.get_sketch();
    }
};
#endif
