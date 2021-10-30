//
// Created by gil on 16/01/20.
//

#ifndef SECURE_REPORT_CONFIG_H
#define SECURE_REPORT_CONFIG_H

#include <string>

// TODO
// 1. Rename PROCESSED_FILE_PATH to PROCESSED_DIR_PATH. Save table vector to PROCESSED_DIR_PATH
// 2. Rename paths relatively to project

namespace constants {
    const bool OUTPUT_TO_CONSOLE = true;
    const std::string PROCESSED_DIR_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/";
//    const std::string TABLE_VECTOR_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/";
    const std::string OUTPUT_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/output_file.txt";
    const std::string PROCESSED_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/";
    const std::string KEY_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/key";
    enum OUTPUT_LEVELS {ERROR, REGULAR, LOG, DEBUG};
    const int OUTPUT_LEVEL = OUTPUT_LEVELS::LOG;
    const int WORD_LENGTH = 1;
    const int SIMD = 1;
}

#endif //SECURE_REPORT_CONFIG_H
