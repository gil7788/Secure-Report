//
// Created by gil on 09/05/20.
//

#ifndef SECURE_REPORT_TRUSTEDTHIRDPARTY_H
#define SECURE_REPORT_TRUSTEDTHIRDPARTY_H


#include "SketchEncoder.h"

class TrustedThirdParty {
public:

    int _database_size;
    int _maximal_number_of_matches_per_query;
    SketchEncoder _encoder;

    TrustedThirdParty(int database_size, int maximal_number_of_matches_per_query);
};


#endif //SECURE_REPORT_TRUSTEDTHIRDPARTY_H
