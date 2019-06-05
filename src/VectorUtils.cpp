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