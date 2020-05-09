#ifndef SECURE_REPORT_PLAINDATABASE_H
#define SECURE_REPORT_PLAINDATABASE_H

#include <mysql++.h>
#include <iostream>
#include <cstring>
#include <string>
#include <Eigen/Dense>

#include "DatabaseConstants.h"

using Eigen::MatrixXi;
using Eigen::VectorXi;

using namespace mysqlpp;
using namespace std;

// DEBUG_LEVEL in [0,1,2], 0 - lowest debug level(none debug messages), 2 - all debug messages
#define DEBUG_LEVEL 0

class PlainDatabase {
private:
    Connection _connection;
    const string _USERNAME = "root";
    const string _PASSWORD = "qwe123";
    const string _LOCALHOST = "127.0.0.1:3306";
    const string _DB_NAME = "sys";

private:
    bool insert(string table_name, int value);
    bool create_table(string table_name);
    bool truncate_table(string& table_name);
    bool fill_table(string& table_name, std::vector<int>& init_vector);
    void print_sql_exception(mysqlpp::BadQuery& e, string query);
    bool execute_sql_query(string query);
public:
    bool connect();
    bool initiate_table(string table_name, std::vector<int>& init_vector);
    bool table_exists(const string& table_name);
    bool update_table(string table_name, std::vector<int>& vector);
    VectorXi table_to_vector(const string& db_table_name);
    std::string table_to_string(string table_name);
};


#endif
