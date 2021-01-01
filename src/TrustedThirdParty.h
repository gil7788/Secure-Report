//
// Created by gil on 09/05/20.
//

#ifndef SECURE_REPORT_TRUSTEDTHIRDPARTY_H
#define SECURE_REPORT_TRUSTEDTHIRDPARTY_H


#include "SketchEncoder.h"

using namespace std;

class TrustedThirdParty {
public:
    vector<SketchEncoder> _encoders;

    void construct_sketch_matrices();

    void initialize(int database_size, vector<int>& matrices_sparsity);

    SketchEncoder get_matrix_by_index(int i);
};


#endif //SECURE_REPORT_TRUSTEDTHIRDPARTY_H
