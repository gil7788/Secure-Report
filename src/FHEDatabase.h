#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "SimplifiedHelibNumber.h"
#include "GenericZP.h"
#include "utils/mathmatical/VectorUtils.h"
#include "Config.h"
#include "utils/file_system/InputOutput.h"
#include "PlainFileSystemDatabase.h"

/**
 * Specific use of database API
 * @tparam Number Plain/Encrypted Fully Homomorphic number template.
 *
 * @see PlainFileSystemDatabase
 */
template<class Number>
class FHEDatabase {
private:
    int _n;
    PlainFileSystemDatabase<Number> _database;
    // @TODO Export table name initialization out side the class, probably consider Config.h or Run.cpp
    // Update constructor, add table_to_vector, table_to_string, database_table_exists documentation accordingly
    const std::string _TABLE_NAME = "table_vector";
    InputOutput _io;

public:
    /**
     *
     * @param db_size Database size
     * @param io Error output file
     *
     * @see InputOutput::InputOutput
     */
    FHEDatabase(int db_size, InputOutput& io):
            _n(db_size),
            _database(),
            _io(io)
            {}

    /**
     * Connect to database
     * @return Connected to database
     */
    bool connect() {
        bool database_connected = _database.connect();
        return database_connected;
    }

    /**
     * Build database table with unique prime key and value columns.
     * May fail to build database table, error is saved in file path provided in constructor parameter io.
     *
     * @param vector Vector of Fully Homomorphic numbers
     * @return Database table is built
     *
     * @see FHEDatabase::FHEDatabase
     * @see InputOutput
     */
    bool build_database_table(std::vector<Number>& vector) {
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

    /**
     * Convert database table to vector of Fully Homomorphic numbers
     * @return Vector of Fully Homomorphic numbers
     */
    std::vector<Number> table_to_vector() {
        std::vector<Number> vectorized_database = _database.table_to_vector(_TABLE_NAME);
        return vectorized_database;
    }

    /**
     * Convert database table to string of Fully Homomorphic numbers
     * @return String of Fully Homomorphic numbers
     */
    std::string table_to_string() {
        return _database.table_to_string(_TABLE_NAME);
    }

    bool database_table_exists() {
        return _database.table_exists(_TABLE_NAME);
    }
};
#endif
