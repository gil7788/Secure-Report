//
// Created by gil on 09/05/20.
//

#include "TrustedThirdParty.h"

void TrustedThirdParty::initialize(int database_size, vector<int>& matrices_sparsity) {
    for(auto& sparsity: matrices_sparsity) {
        SketchEncoder encoder_at_i{database_size, sparsity};
        _encoders.push_back(encoder_at_i);
    }
}

void TrustedThirdParty::construct_sketch_encoders() {
    for(auto& encoder: _encoders) {
        encoder.construct_sketch_matrix();
    }
}

SketchEncoder TrustedThirdParty::get_encoder_by_index(int i) {
    return _encoders[i];
}
