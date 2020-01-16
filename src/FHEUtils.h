#ifndef SECURE_REPORT_FHEUTILS_H
#define SECURE_REPORT_FHEUTILS_H

#include <Eigen/Dense>
#include <helib_number.h>
#include <binomial_tournament.h>

using namespace std;
using Eigen::VectorXi;

template<class Number>
class FHEUtils{
public:
    /* TODO
     * Implement few comparison functions, such as:
     *  1. Equality - Done
     *  2. Less or equals
     *  3. Less than
     *  4. Greater or equals
     *  5. Greater
     * */

    static Number areEqualBinary(Number* x, Number* y, int length) {
        MulBinomialTournament<Number> product_heap = MulBinomialTournament<Number>();
        for (int i = 0; i < length; ++i) {
            Number result_at_i = Number::static_from_int(1) - (x[i] - y[i]);
            product_heap.add_to_tournament(result_at_i, 0);
        }

        Number result = product_heap.unite_all(MulBinomialTournament<Number>::mul);
        return result;
    }

    static VectorXi encryptVectorXi(VectorXi plainVector);

    static Number* encryptArray(Number* plainVector);

    static Number test();


};
#endif //SECURE_REPORT_FHEUTILS_H