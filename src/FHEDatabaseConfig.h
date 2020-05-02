//
// Created by gil on 28/04/20.
//

#ifndef SECURE_REPORT_FHEDATABASECONFIG_H
#define SECURE_REPORT_FHEDATABASECONFIG_H

#include <iostream>
#include "Config.h"
#include "InputOutput.h"

enum class DATA_TYPES {PLAIN, ENCRYPTED, BOTH};

class DatabaseDataType {
    public:

    virtual DATA_TYPES get_data_type() = 0;

    virtual void initialize_data_type() = 0;

    virtual ~DatabaseDataType() {};
};

class PlainDataType: public DatabaseDataType {
public:
    const DATA_TYPES _data_type = DATA_TYPES ::PLAIN;
    int _database_matches_sparsity;

    PlainDataType(int database_matches_sparsity);

    DATA_TYPES get_data_type() override;

    void initialize_data_type() override;
};

class EncryptedDataType: public DatabaseDataType {
public:
    int _database_size;
    long _s;
    long _R;
    long _r;
    long _d;
    long _c;
    long _k;
    int _L;
    long _chosen_m;
    Vec<long>& _gens;
    Vec<long>& _ords;
    HelibKeys _keys;

    EncryptedDataType(int database_size, long s, long R, long r,
                      long d, long c, long k, int L, long chosen_m, Vec<long>& gens, Vec<long>& ords);

    DATA_TYPES get_data_type() override;

    void initialize_data_type() override;
};
#endif //SECURE_REPORT_FHEDATABASECONFIG_H
