//
// Created by gil on 09/05/20.
//

#ifndef SECURE_REPORT_TRUSTEDTHIRDPARTY_H
#define SECURE_REPORT_TRUSTEDTHIRDPARTY_H


#include "SketchEncoder.h"

using namespace std;

class TrustedThirdParty {
public:

    int _database_size;
    vector<int> _matrices_sparsity;
    vector<SketchEncoder> _encoders;

    TrustedThirdParty(int database_size, int maximal_number_of_matches_per_query);

    TrustedThirdParty(int database_size, vector<int>& matrices_sparsity);

    void initialize();

    SketchEncoder get_matrix_by_index(int i);

    VectorXi encode_with_i_matrix(VectorXi& vector, int i);

    VectorXi decode_with_i_matrix(VectorXi& vector, int i);

    int get_number_of_matrices();
};


#endif //SECURE_REPORT_TRUSTEDTHIRDPARTY_H
