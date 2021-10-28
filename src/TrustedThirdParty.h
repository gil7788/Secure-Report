//
// Created by gil on 09/05/20.
//

#ifndef SECURE_REPORT_TRUSTEDTHIRDPARTY_H
#define SECURE_REPORT_TRUSTEDTHIRDPARTY_H


#include "SketchEncoder.h"

using namespace std;

/**
 * Implement API for Sketch Encoders (see SketchEncoder).
 */
class TrustedThirdParty {
public:
    vector<SketchEncoder> _encoders;

    /**
     * Construct initialized encoders (first initialize encoders by TrustedThirdParty::initialize)
     */
    void construct_sketch_matrices();

    /**
     * Initialize Sketch Encoders (see SketchEncoder)
     * @param database_size Used as number of rows in the encoder
     * @param matrices_sparsity Each sparsity is iterated,
     * and Sketch Encoder initialized with (database_size, matrices_spatsity[i])
     */
    void initialize(int database_size, vector<int>& matrices_sparsity);

    /**
     * Get SketchEncoder at i
     * @param i Encoder's index
     * @return SketchEncoder at i
     */
    SketchEncoder get_matrix_by_index(int i);
};


#endif //SECURE_REPORT_TRUSTEDTHIRDPARTY_H
