#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SimplifiedHelibNumber.h"
#include "GenericZP.h"
#include "utils/mathmatical/VectorUtils.h"
#include "Config.h"
#include "utils/file_system/InputOutput.h"
#include "PlainFileSystemDatabase.h"


template<class Number>
class FHEDatabase {
private:
    int _n;
    PlainFileSystemDatabase<Number> _database;
    const std::string _TABLE_NAME = "table_vector";
    InputOutput _io;

public:
    FHEDatabase(int db_size, InputOutput& io):
            _n(db_size),
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

    bool build_database_table(std::vector<Number>& vector) {
        /*
         * Description: Build database table with unique prime key and value columns
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

    std::vector<Number> table_to_vector() {
        std::vector<Number> vectorized_database = _database.table_to_vector(_TABLE_NAME);
        return vectorized_database;
    }

    std::string table_to_string() {
        return _database.table_to_string(_TABLE_NAME);
    }

    bool database_table_exists() {
        return _database.table_exists(_TABLE_NAME);
    }
};
#endif
