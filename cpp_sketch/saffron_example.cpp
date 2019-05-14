#include <iostream>
#include <Eigen/Dense>

#include "sketch_matrix.h"

using namespace Eigen;
using Eigen::MatrixXi;
using Eigen::VectorXi;

int main() {
    int n = 1028, d = 3;
    sketch_matrix sketch_test(n, d);

    // Params
    const double Ce = 11.36;
    int _n = n;
    int _d = d;
    int _L = int(std::ceil(std::log2(n)));
    int _h = _L * int(std::ceil(Ce * d));
    int _m = int(std::floor(_h / d));

    std::cout << "Params: " << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "d: " << d << std::endl;
    std::cout << "Ce: " << Ce << std::endl;
    std::cout << "_L: " << _L << std::endl;
    std::cout << "_n: " << _n << std::endl;
    std::cout << "_d: " << _d << std::endl;
    std::cout << "_h: " << _h << std::endl;
    std::cout << "_m: " << _m << std::endl;
    // Matrices sizes
    std::cout << "_H: " << sketch_test._H.rows() << ", " << sketch_test._H.cols() << std::endl;
    std::cout << "_M: " << sketch_test._M.rows() << ", " << sketch_test._M.cols() << std::endl;
    std::cout << "_U: " << sketch_test._U.rows() << ", " << sketch_test._U.cols() << std::endl;

    std::cout << "=====================================" << std::endl;
    std::cout<< "TEST 001 - Encode" << std::endl;
    VectorXi input;
    input= VectorXi::Zero(n);
    input(6) = 1;
    input(17) = 1;

    VectorXi encoded = sketch_test.encode(input);
    std::cout << "encoded.maxCoeff(): " << encoded.maxCoeff() << std::endl;
    std::cout << "encoded.minCoeff(): " << encoded.minCoeff() << std::endl;
    std::cout << "encoded hamming weight: " << encoded.sum() << std::endl;
    std::cout << "encoded.shape: " << encoded.rows() << ", " << encoded.cols() << std::endl;

    std::cout << "=====================================" << std::endl;
    std::cout<< "TEST 002 - Decode" << std::endl;
    VectorXi decoded = sketch_test.decode(encoded);
    std::cout << "decoded: " << std::endl << decoded << std::endl;

    std::cout << "decoded.maxCoeff(): " << decoded.maxCoeff() << std::endl;
    std::cout << "decoded.minCoeff(): " << decoded.minCoeff() << std::endl;
    std::cout << "decoded hamming weight: " << decoded.sum() << std::endl;
    std::cout << "decoded.shape: " << decoded.rows() << ", " << decoded.cols() << std::endl;
}
