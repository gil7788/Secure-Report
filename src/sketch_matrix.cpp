#include "sketch_matrix.h"


sketch_matrix::sketch_matrix(int n, int d) {}

sketch_matrix::sketch_matrix(int n, int d, int L, int h, MatrixXi U,MatrixXi matrix) {}

VectorXi sketch_matrix::encode(VectorXi vector) {
/*
    input: binary vector x with d 1s
    output: binary vector
    description: returns y = Mx
    running time: O(n+d^3*logn)
*/
}

VectorXi sketch_matrix::decode(VectorXi vector) {
/*
    input: binary vector y=Mx
    output: binary vector
    description: extracts x from y
    running time: O(d^2*polylog(n))
    implement the psuedo code on page 17 on the paper on:
    https://pdfs.semanticscholar.org/c051/52418cc0a666eaa333efc119ea3c639279b1.pdf
 */
}

VectorXi sketch_matrix::number_to_binary(int i) {
// Returns number i's binary representation
/*
    input: int i, int n
    output: array of size log(n)
    description: returns the binary representation of i with padding to n
    running time: O(log(n))
*/
}

int sketch_matrix::binary_to_number(VectorXi vector) {
    // base10
/*
    input: binary array
    output: int
    description: returns the decimal value the array represents
    running time: O(|b_list|)
*/
}

int sketch_matrix::hamming_weight(VectorXi vector) {
// Returns the hamming weight of vector
/*
    input: binary vector v
    output: int
    description: returns the hamming weight of v
    running time: O(|v|)
 */
}

VectorXi sketch_matrix::square_plus(VectorXi  vector_a, VectorXi  vector_b) {
/*
    input: 2 binary vectors a,b
    output: vector over {0,1,?} when ? is 2 or 3
    description: returns b if a=0 or unknown if a=1
    running time: O(|a|)
 */
}

VectorXi sketch_matrix::line_plus(VectorXi vector_a, VectorXi vector_b) {
/*
    input: 2 vectors a,b over {0,1,?} when ? is 2 or 3
    output: binary vector
    description: returns ~a if b is unknown or b otherwise
    running time: O(|a|)
*/
}

MatrixXi sketch_matrix::tensor_product(MatrixXi matrix_a, MatrixXi matrix_b) {
/*
    input: h by n incidence matrix H of type np.matrix, k by n signature matrix U of type np.matrix
    output: h*k by n d-disjunct matrix of type np.matrix
    description: return the tensor product of H and U
    running time: O(hkn)
 */
}

MatrixXi sketch_matrix::signature_matrix() {
/*
    input: int n
    output: 6log(n) by n signature matrix of type np.array
    description: return the tensor product of H and U
    running time: O(nlogn)
 */
}

MatrixXi sketch_matrix::incidence_matrix(int m) {
/*
    input: none
    output: h by n matrix of type np.array
    description: return the incidence matrix of a left regular bipartite graph
    running time: O(hn)
 */
}