#ifndef SECURE_REPORT_SKETCH_MATRIX_H
#define SECURE_REPORT_SKETCH_MATRIX_H

#include <iostream>
#include <Eigen/Dense>
#include <iterator>

using Eigen::MatrixXi;
using Eigen::VectorXi;


/*
    M: d-disjunct matrix denoted as sketch of type MatrixXi
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

class SketchEncoder {
// Class variables
private:
    const double Ce = 11.36;
    int _n;
    int _d;
    int _L;
    int _h;
    int _m;

    VectorXi _pi3_inv;
    VectorXi _pi5_inv;

    MatrixXi _U;
    MatrixXi _H;
    MatrixXi _sketch;

// Class interface
public:
    SketchEncoder(int n, int d);

    MatrixXi get_sketch();

    VectorXi encode(VectorXi vector);

    VectorXi decode(VectorXi vector);

    void construct_sketch_matrix();

// Helper functions
private:
    VectorXi number_to_binary(int i);

    int binary_to_number(VectorXi vector);

    static VectorXi generate_binary_vector(int n, int d);

private:
    int hamming_weight(VectorXi vector);

    // Operations on 2 vectors
    static VectorXi square_plus(VectorXi vector_a, VectorXi vector_b);

    static VectorXi line_plus(VectorXi vector_a, VectorXi vector_b);

    // Operations on 2 matrices
    static MatrixXi tensor_product(MatrixXi matrix_a, MatrixXi matrix_b);

    MatrixXi signature_matrix();

    MatrixXi incidence_matrix();

    std::vector<int> find_singletons(VectorXi vector, VectorXi* vector_tag, int* count);

    VectorXi find_doubletons(std::vector<int> G, VectorXi vector_tag, int count);

    static VectorXi generate_permutation(int n);

    static VectorXi sort_indirect(VectorXi x);

    static VectorXi bitwise_not(VectorXi x);

    friend std::ostream& operator<< (std::ostream& stream, const SketchEncoder& matrix);
};

#endif //SECURE_REPORT_SKETCH_MATRIX_H
