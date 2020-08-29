//
// Created by gil on 09/05/20.
//

#include "TrustedThirdParty.h"


TrustedThirdParty::TrustedThirdParty(int database_size, int maximal_number_of_matches_per_query):
        _database_size{database_size},
        _matrices_sparsity{maximal_number_of_matches_per_query},
        _encoders{SketchEncoder{database_size, maximal_number_of_matches_per_query}} {}

TrustedThirdParty::TrustedThirdParty(int database_size, vector<int>& matrices_sparsity):
        _database_size{database_size},
        _matrices_sparsity{matrices_sparsity} {
    for (int i = 0; i < matrices_sparsity.size(); ++i) {
        SketchEncoder encoder_at_i{database_size, matrices_sparsity[i]};
        _encoders.push_back(encoder_at_i);
    }
}

void TrustedThirdParty::initialize() {
    for(auto& encoder: _encoders) {
        encoder.construct_sketch_matrix();
    }
}

SketchEncoder TrustedThirdParty::get_matrix_by_index(int i) {
    return _encoders[i];
}

VectorXi TrustedThirdParty::encode_with_i_matrix(VectorXi& vector, int i) {
    auto encoder = get_matrix_by_index(i);
    return encoder.encode(vector);
}

VectorXi TrustedThirdParty::decode_with_i_matrix(VectorXi& vector, int i) {
    auto encoder = get_matrix_by_index(i);
    return encoder.decode(vector);
}

int TrustedThirdParty::get_number_of_matrices() {
    return _encoders.size();
}