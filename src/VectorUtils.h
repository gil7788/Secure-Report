#ifndef SECURE_REPORT_CONVERSION_UTILS_H
#define SECURE_REPORT_CONVERSION_UTILS_H

#include <Eigen/Dense>

using Eigen::MatrixXi;
using Eigen::VectorXi;


class VectorUtils {
public:
    static VectorXi number_to_binary(int i, int n);

    static int binary_to_number(VectorXi vector);

    static VectorXi generate_binary_vector(int n, int d);

    static std::vector<int> getMatches(VectorXi x);
};


#endif
