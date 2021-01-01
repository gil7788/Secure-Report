//
// Created by gil on 28/04/20.
//

#ifndef SECURE_REPORT_FHEDATABASECONFIG_H
#define SECURE_REPORT_FHEDATABASECONFIG_H

#include <iostream>
#include <experimental/filesystem>

#include "GenericZP.h"
#include "SimplifiedHelibKeys.h"
#include "SimplifiedHelibNumber.h"

namespace fs = std::experimental::filesystem;

enum class DATA_TYPES {PLAIN, ENCRYPTED, MIXED};

class DatabaseDataType {
    public:

    virtual DATA_TYPES get_data_type() = 0;

    virtual void initialize() = 0;

    virtual void write_key_to_file(const std::string& key_file_path) = 0;

    virtual void read_key_from_file(const std::string& key_file_path) = 0;

    virtual ~DatabaseDataType() = default;
};

class GenericPlainDataType: public DatabaseDataType {
public:
    const DATA_TYPES _data_type = DATA_TYPES ::PLAIN;
    int _r;

    explicit GenericPlainDataType(int r);

    DATA_TYPES get_data_type() override;

    virtual void write_key_to_file(const std::string& key_file_path) override;

    virtual void read_key_from_file(const std::string& key_file_path) override;

    void initialize() override;
};

class EncryptedDataTypeFromParameters: public DatabaseDataType {
    int _database_size;
    long _s;
    long _R;
    long _r;
    long _d;
    long _c;
    long _k;
    int _L;
    long _chosen_m;
    Vec<long> _gens;
    Vec<long> _ords;
    SimplifiedHelibKeys _keys;
    const fs::path _key_file_path;

public:
    EncryptedDataTypeFromParameters(int database_size, long s, long R, long r,
                                    long d, long c, long k, int L, long chosen_m, Vec<long>& gens, Vec<long>& ords,
                                    const std::string& key_file_path);

    DATA_TYPES get_data_type() override;

    void initialize() override;

    void write_key_to_file(const std::string& key_file_path) override;

    void read_key_from_file(const std::string& key_file_path) override;
};
#endif //SECURE_REPORT_FHEDATABASECONFIG_H
