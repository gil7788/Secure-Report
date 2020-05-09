//
// Created by gil on 09/05/20.
//

#include "TrustedThirdParty.h"


TrustedThirdParty::TrustedThirdParty(int database_size, int maximal_number_of_matches_per_query):
        _database_size{database_size},
        _maximal_number_of_matches_per_query{maximal_number_of_matches_per_query},
        _encoder{database_size, maximal_number_of_matches_per_query} {}