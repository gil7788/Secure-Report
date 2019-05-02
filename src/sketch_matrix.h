#ifndef SECURE_REPORT_SKETCH_MATRIX_H
#define SECURE_REPORT_SKETCH_MATRIX_H

#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXi;
using Eigen::VectorXi;

/*
    M: d-disjunct matrix
    U: signature matrix
    d: number of defectives items
    h: number of right nodes- number of bundles of tests
    k: number of rows on U
    n: number of items
    L: log (base 2) of n
    x: vector of set of items
    y: M*x
    H: incidence matrix, used to build M
    m: number of 1's on each row of H
    G: set of indices after decode
    Ce: used to determine the success of rate of the decoding
 * */

class sketch_matrix {
    // TODO:
    /*
     * 1. Define class variables on demand
     * */
public:
    // Build class from matrix
    sketch_matrix(int n, int d);

    // Build class from params
    sketch_matrix(int n, int d, int L, int h, MatrixXi U,MatrixXi matrix);

    VectorXi encode(VectorXi vector);

    VectorXi decode(VectorXi vector);

// Class variables
private:

// Helper functions
private:
    // Operations on single number
    VectorXi number_to_binary(int i);

    // Operations on single vector
    int binary_to_number(VectorXi vector);

    int hamming_weight(VectorXi vector);

    // Operations on 2 vectors
    VectorXi square_plus(VectorXi vector_a, VectorXi vector_b);

    VectorXi line_plus(VectorXi vector_a, VectorXi vector_b);

    // Operations on 2 matrices
    MatrixXi tensor_product(MatrixXi matrix_a, MatrixXi matrix_b);

    MatrixXi signature_matrix();

    MatrixXi incidence_matrix(int m);
};


#endif //SECURE_REPORT_SKETCH_MATRIX_H
