#include <iostream>
#include "VectorUtils.h"

VectorXi VectorUtils::number_to_binary(int value, int n) {
/*
    input: int i, int n
    output: array of size log(n)
    description: returns the binary representation of i with padding to n
    running time: O(log(n))
*/
    int int_value_current = value;
    int vector_size = int(std::ceil(std::log2(n)));
    VectorXi binary_value_vector(vector_size);

    for(int array_index = vector_size-1; array_index >= 0; --array_index) {
        binary_value_vector(array_index) = (int_value_current % 2 == 0 ? 0 : 1);
        int_value_current /= 2;
    }

    return binary_value_vector;
}

int VectorUtils::binary_to_number(VectorXi vector) {
/*
    input: binary array
    output: int
    description: returns the decimal value the array represents
    running time: O(|b_list|)
*/
    int value = 0;
    long vector_size = vector.size();

    for(int i = vector_size-1, power = 1; i >= 0; --i, power *= 2) {
        value += vector(i) * power;
    }

    return value;
}

std::vector<long> VectorUtils::number_to_std_vector(int value, int length) {
    int int_value_current = value;
    std::vector<long> binary_value_vector;

    for(int i = 0; i < length; ++i) {
        int digit = int_value_current % 2 == 0 ? 0 : 1;
        binary_value_vector.insert(binary_value_vector.cbegin(), digit);
        int_value_current /= 2;
    }

    return binary_value_vector;
}

long VectorUtils::std_vector_to_number(std::vector<long>& binary_representation) {
    long value = 0;
    for(long i = (long) binary_representation.size() - 1, power = 1; i >= 0; --i, power *= 2) {
        value += binary_representation[i] * power;
    }
    return value;
}

VectorXi VectorUtils::generate_binary_vector(int n, int d) {
    /*
     * Generate binary vector of size n and hamming weight d
     * */
    int p = n;
    int t = d;
    VectorXi result(p);
    result = VectorXi::Zero(p);

    //Get a random position in the 1xp matrix from 0-p
    for (int i = 0; i < t; ++i)
    {
        // Define random number generator
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dis(0, p-1);

        // Generate values until new unique index is generated
        int randPos = dis(mt) % p;
        while (result[randPos] == 1)
            randPos = dis(mt) % p;

        //Change the random position from a 0 to a 1
        result[randPos] = 1;
    }

    return result;
}

std::vector<int> VectorUtils::generate_binary_std_vector(int vector_size, int value_occurences) {
    /*
     * Generate binary vector of size n and hamming weight d
     * */
    return generate_int_vector(vector_size, value_occurences, 1);
}

std::vector<int> VectorUtils::generate_int_vector(int vector_size, int value_occurences, int value) {
    /*
     * Generate binary vector of size n and hamming weight d
     * */

    std::vector<int> result(vector_size, 0);
//    result = VectorXi::Zero(p);

    //Get a random position in the 1xp matrix from 0-p
    for (int i = 0; i < value_occurences; ++i)
    {
        // Define random number generator
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dis(0, vector_size-1);

        // Generate values until new unique index is generated
        int randPos = dis(mt) % vector_size;
        while (result[randPos] == value)
            randPos = dis(mt) % vector_size;

        //Change the random position from a 0 to a 1
        result[randPos] = value;
    }

    return result;
}


std::vector<int> VectorUtils::getMatches(VectorXi x) {
    /*
     * Helper function for binary vectors, returns "True" (1) indices
     * */
    std::vector<int> indices;
    for (int i = 0; i < x.size(); ++i) {
        if(x[i] == 1) {
            indices.insert(indices.begin(), i);
        }
    }
    return indices;
}

std::vector<int> VectorUtils::get_matches_std_vector(std::vector<int> binary_vector, int comparison_value) {
    /*
     * Helper function for binary vectors, returns "True" (1) indices
     * */
    std::vector<int> matches_indices;

    for(int i =0; i < binary_vector.size(); ++i) {
        if(binary_vector[i] == comparison_value) {
            matches_indices.push_back(i);
        }
    }
    return matches_indices;
}

std::string VectorUtils::to_string(VectorXi vector) {
    /*
     * VectorXi to std::string converter
     * */
    std::string result;
    for (int i = 0; i < vector.size(); ++i) {
        result += std::to_string(vector(i)) + "\n";
    }
    return result;
}

std::vector<int> VectorUtils::eigen_vector_to_std_vector(VectorXi eigen_vector) {
    std::vector<int> std_vector(eigen_vector.data(), eigen_vector.data() + eigen_vector.size());
    return std_vector;
}
