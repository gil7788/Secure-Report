//
// Created by gil on 29/04/20.
//

#ifndef SECURE_REPORT_DATABASE_H
#define SECURE_REPORT_DATABASE_H

#include <zp.h>
#define SIMD_FACTOR 1
typedef ZP<SIMD_FACTOR> MyZP;

#include "FHEDatabase.h"
#include "FHEDatabaseConfig.h"

template <typename DataType>
class Database {
public:
    int _size;
    int _sparsity;

    Database(int size, int sparsity);

    virtual FHEDatabase<DataType> build_database() = 0;

    virtual void initialize_data_type() = 0;

    virtual ~Database() {};
};


class RegularDatabase: public Database<MyZP> {
public:

    PlainDataType _data_type;

    RegularDatabase(int size, int sparsity);

    FHEDatabase<MyZP> build_database() override;

    void initialize_data_type() override;
};

class EncryptedDatabase: public Database<HelibNumber> {
public:
    EncryptedDataType& _data_type;

    EncryptedDatabase(int size, int sparsity, EncryptedDataType& data_type);

    FHEDatabase<HelibNumber> build_database() override;

    void initialize_data_type() override ;
};
#endif //SECURE_REPORT_DATABASE_H
