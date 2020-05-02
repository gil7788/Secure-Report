//
// Created by gil on 16/01/20.
//

#ifndef SECURE_REPORT_CONFIG_H
#define SECURE_REPORT_CONFIG_H

#include <string>


static const bool OUTPUT_TO_CONSOLE = true;
static const std::string OUTPUT_FILE_PATH = "/home/gil/University/semester19a/crypto_lab/work/secure_report/processed/output_file.txt";
enum OUTPUT_LEVELS {ERROR, REGULAR, LOG, DEBUG};
static const int OUTPUT_LEVEL = OUTPUT_LEVELS::REGULAR;


class Config {
public:

};

#endif //SECURE_REPORT_CONFIG_H
