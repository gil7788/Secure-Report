//
// Created by gil on 29.7.2020.
//

#ifndef SECURE_REPORT_PLAINFILESYSTEMDATABASE_H
#define SECURE_REPORT_PLAINFILESYSTEMDATABASE_H

#include <mysql++.h>
#include <iostream>
#include <cstring>
#include <string>
#include <Eigen/Dense>
#include <vector>
#include <experimental/filesystem>
#include "utils/file_system/InputOutput.h"
#include "Config.h"
#include "utils/mathmatical/VectorUtils.h"
#include <iosfwd>
#include <fstream>

using namespace std;
using Eigen::MatrixXi;
using Eigen::VectorXi;
namespace fs = std::experimental::filesystem;

/**
 * API for database creation
 * @tparam Number Plain/Encrypted Fully Homomorphic number template.
 */
template<class Number>
class PlainFileSystemDatabase {
private:
    int _table_counter = 0;
private:
    bool insert(const string& table_name, Number value) {
        string value_file_path = table_name + "/" + to_string(_table_counter);
        InputOutput io = InputOutput(false, value_file_path, constants::OUTPUT_LEVEL);
        stringstream stringified_value;
        stringified_value << value;
        bool saved_to_file_system = io.output(stringified_value.str(), constants::ERROR);
        if(! saved_to_file_system) {
            cout << "Failed to insert element at index: " << to_string(_table_counter) << ".\n";
            return false;
        }
        _table_counter = _table_counter + 1;
        return true;
    };

//    @TODO Consider to delete, not implemented
    bool create_table(string table_name);
    bool truncate_table(string& table_name);
    bool fill_table(string& table_name, std::vector<Number>& init_vector);
    void print_sql_exception(mysqlpp::BadQuery& e, string query);
    bool execute_sql_query(string query);
public:
    bool connect() {
        return true;
    }

    bool initiate_table(const string& table_name, std::vector<Number>& init_vector) {
        return update_table(table_name, init_vector);
    }

    bool table_exists(const string& table_name) {
        auto absolute_table_name = fs::absolute(table_name);
        return fs::exists(absolute_table_name) && fs::is_directory(absolute_table_name);
    }

    bool update_table(const string& table_name, std::vector<Number>& vector) {
        for (auto& element: vector) {
            bool is_inserted = insert(table_name, element);
            if(! is_inserted) {
                return false;
            }
        }
        return true;
    }
    vector<Number> table_to_vector(const string& db_table_name) {
        vector<Number> vectorized_database;
        for (int element_index = 0; element_index < _table_counter; ++element_index) {
            string file_path = db_table_name + "/" + to_string(element_index);
            std::filebuf fb;
            if (! fb.open (file_path,std::ios::in)) {
                std::cout << "Failed to read from database \n";
            }
            else {
                std::istream is(&fb);
                Number database_element;

                is >> database_element;
                vectorized_database.push_back(database_element);
            }
        }
        return vectorized_database;
    }

    std::string table_to_string(const string& table_name) {
        vector<Number> table_vector = table_to_vector(table_name);
        vector<int> plain_table_vector;
        for (int i = 0; i < table_vector.size(); ++i) {
            int x = table_vector.at(i).to_int();
            plain_table_vector.push_back(x);
        }

        return VectorUtils::std_vector_to_string<int>(plain_table_vector);
    }
};


#endif //SECURE_REPORT_PLAINFILESYSTEMDATABASE_H
