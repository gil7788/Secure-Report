#ifndef SECURE_REPORT_CONVERSION_UTILS_H
#define SECURE_REPORT_CONVERSION_UTILS_H

#include <Eigen/Dense>

using Eigen::MatrixXi;
using Eigen::VectorXi;


class VectorUtils {
public:
    static VectorXi number_to_binary(int i, int n);

    static int binary_to_number(VectorXi vector);

    static std::vector<long> number_to_std_vector(int value, int length);

    static long std_vector_to_number(std::vector<long>& binary_representation);

    static VectorXi generate_binary_vector(int n, int d);

    static std::vector<int> generate_binary_std_vector(int n, int d);

    static std::vector<int> generate_int_vector(int vector_size, int value_occurences, int value);

    static std::vector<int> getMatches(VectorXi x);

    static std::vector<int> get_matches_std_vector(std::vector<int> x, int comparison_value);

    static std::string to_string(VectorXi vector);

    template <typename DataType>
    static std::string std_vector_to_string(std::vector<DataType>& vector) {
        std::stringstream string_stream;
        for(auto& elem: vector) {
            string_stream << elem << " ";
        }

        return string_stream.str();
    }

    static std::vector<int> eigen_vector_to_std_vector(VectorXi eigen_vector);
};


#endif
