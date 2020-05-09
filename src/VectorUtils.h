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

    static std::vector<int> generate_binary_std_vector(int n, int d);

    static std::vector<int> getMatches(VectorXi x);

    static std::vector<int> get_matches_std_vector(std::vector<int> x);

    static std::string to_string(VectorXi vector);

    static std::string std_vector_to_string(std::vector<int>& x);

    static std::vector<int> eigen_vector_to_std_vector(VectorXi eigen_vector);
};


#endif
