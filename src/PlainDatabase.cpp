#include "PlainDatabase.h"


bool PlainDatabase::connect() {
    /*
    * Description: Connect to database
    * */
    try {
        _connection = mysqlpp::Connection(_DB_NAME.c_str(), _LOCALHOST.c_str(), _USERNAME.c_str(), _PASSWORD.c_str());
        return true;
    }
    catch(mysqlpp::ConnectionFailed& e) {
        cerr << "Failed to connect to database: " << _DB_NAME.c_str()
        << "\n Host: " << _LOCALHOST.c_str()
        << "\n Username: " << _USERNAME.c_str();

        throw e.what();
    }
}

// Queries
bool PlainDatabase::insert(string table_name, int value) {
    /*
    * Description: Insert to table_name table value
    * */
    /*
    * TODO
    * 1. Expand to costume class table structure
    * */
    string query = "INSERT INTO " + table_name + " (VALUE) " + " VALUES " + "(" + to_string(value) + ")";
    bool executed_successfully = execute_sql_query(query);
    return executed_successfully;
}

bool PlainDatabase::fill_table(string &table_name, std::vector<int>& vector) {
    /*
    * Description: Fill table_name table with vector
    * */
    /*
    * TODO
    * 1. Expand to costume class table structure
    * */
    for (int i = 0; i < vector.size(); ++i) {
        bool inserted_successfully = insert(table_name, vector[i]);
        if(!inserted_successfully) {
            cerr << "Failed to initiate table, table name: " << table_name << "\n" << endl;
            return false;
        }
    }
    return true;
}

bool PlainDatabase::initiate_table(string table_name, std::vector<int>& init_vector) {
    /*
   * Description: Create table_name table and initiate it with VectorXi
   * */
    /*
   * TODO
   * 1. Expand to costume class table structure
   * */
    bool table_created = create_table(table_name);
    if(!table_created) {
        return false;
    }

    bool table_filled = fill_table(table_name, init_vector);
    return table_filled ;
}

bool PlainDatabase::create_table(string table_name) {
    /*
   * Description: Create table_name table with ID (INT unique, primay key auto incremental), VALUE (INT) columns
   * */
    /*
   * TODO
   * 1. Expand to costume class table structure
   * */
    string query = "CREATE TABLE `" + _DB_NAME + "`.`" + table_name + "` " +
                   "(`ID` INT UNSIGNED NOT NULL AUTO_INCREMENT, `VALUE` INT NOT NULL, PRIMARY KEY (`ID`))";

    if (table_exists(table_name)) {
        cerr << "Create table failed. " << table_name << " table already exists" << endl;
        return false;
    }

    bool executed_successfully = execute_sql_query(query);

    if(DEBUG_LEVEL >= 2) {
        std::string table_string = table_to_string(table_name);
        std::cout << table_string << std::endl;
    }

    return executed_successfully;
}

bool PlainDatabase::truncate_table(string &table_name) {
    /*
    * Description: Truncate table called table_name
    * */
    /*
    * TODO
    * 1. Expand to costume class table structure
    * */

    string query = "TRUNCATE TABLE `" + _DB_NAME + "`.`" + table_name + "`";
    bool executed_successfully = execute_sql_query(query);

    if(DEBUG_LEVEL >= 2) {
        std::string table_string = table_to_string(table_name);
        std::cout << table_string << std::endl;
    }

    return executed_successfully;
}

bool PlainDatabase::update_table(string table_name, std::vector<int>& vector) {
    /*
    * Description: Rewrite/Create new table with table_name
    * */
    /*
    * TODO
    * 1. Expand to costume class table structure
    * */
    bool table_dropped = truncate_table(table_name);
    if(!table_dropped)
        return false;
    bool new_table_initiated = fill_table(table_name, vector);
    return new_table_initiated;
}

bool PlainDatabase::table_exists(const string& table_name) {
    /*
    * Description: Check if table_name table exists in database
    * */
    string query = "SELECT 1 FROM " + table_name + " LIMIT 1";

    if(DEBUG_LEVEL >= 1) {
        cout << query << endl;
    }

    try {
        mysqlpp::Query prepared_query = _connection.query(query);
        prepared_query.store();
        prepared_query.erase_event;

        return true;
    }
    catch(mysqlpp::BadQuery& e) {
        print_sql_exception(e, query);
        return false;
    }
}

// Queries utils
bool PlainDatabase::execute_sql_query(string query) {
    /*
     * Description: General execute sql query wrapper
     * */
    if(DEBUG_LEVEL >= 1) {
        cout << query << endl;
    }

    try {
        mysqlpp::Query prepared_query = _connection.query(query);
        prepared_query.execute();
        return true;
    }
    catch(mysqlpp::BadQuery& e){
        print_sql_exception(e, query);
        return false;
    }
}

void PlainDatabase::print_sql_exception(mysqlpp::BadQuery& e, string query) {
    /*
     * Description: print sql exception, used for debbuging
     * */
    cerr << "============================" << endl;
    cerr << "Query failed - error number: " << e.errnum() << endl;
    cerr << e.what() << endl;
    cerr << query << endl;
    cerr << "============================" << endl;
}

// Get from database
VectorXi PlainDatabase::table_to_vector(const string& db_table_name) {
    /*
    * Description: Convert table_name table to VectorXi
    * */
    /*
   * TODO
   * 1. Expand to costume class table structure
   * */
    string query = "SELECT * FROM " + db_table_name;

    try {
        mysqlpp::Query prepared_query = _connection.query(query);

        mysqlpp::StoreQueryResult r = prepared_query.store();

        int field = r.field_num(COL_VALUE);
        ulong database_rows = r.num_rows();
        VectorXi result_vector(database_rows);
        int index = 0;
        for(mysqlpp::StoreQueryResult::iterator i = r.begin(); i!=r.end();i++, index++) {
            result_vector[index] = i->at(field);
        }
        return result_vector;
    }

    catch(mysqlpp::BadQuery& e) {
        print_sql_exception(e, query);
        return VectorXi();
    }
}

std::string PlainDatabase::table_to_string(string table_name)  {
    /*
   * Description: Convert table_name table to string
   * */
    /*
   * TODO
   * 1. Expand to costume class table structure
   * */
    std::string result_string = "Table " + table_name + ": ";
    VectorXi vector_result = table_to_vector(table_name);
    for (int i = 0; i < vector_result.size() ; ++i) {
        result_string.append(std::to_string(vector_result(i)) + "\n");
    }
    return result_string;
}