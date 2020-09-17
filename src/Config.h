//
// Created by gil on 16/01/20.
//

#ifndef SECURE_REPORT_CONFIG_H
#define SECURE_REPORT_CONFIG_H

#include <string>

namespace constants {
    const bool OUTPUT_TO_CONSOLE = true;
    const std::string OUTPUT_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/output_file.txt";
    const std::string PROCESSED_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/";
    enum OUTPUT_LEVELS {ERROR, REGULAR, LOG, DEBUG};
    const int OUTPUT_LEVEL = OUTPUT_LEVELS::LOG;
    const int WORD_LENGTH = 1;
    const int SIMD = 1;
}

#endif //SECURE_REPORT_CONFIG_H
