#ifndef SECURE_REPORT_FHEUTILS_H
#define SECURE_REPORT_FHEUTILS_H

#include <Eigen/Dense>
#include <helib_number.h>
#include <binomial_tournament.h>
#include "GenericZP.h"
#include "EncryptedBits.h"

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

    static Number areEqualBinary(Number& x, Number& y) {
        auto vec = std::vector<long>(x.r(), 1);
        auto one_vector = VectorUtils::std_vector_to_number(vec);
        Number encrypted_one(one_vector);

        Number result;
        if(encrypted_one.r() <= 1) {
            result = encrypted_one - x + y;
            return result;
        }
        else {
            MulBinomialTournament<Number> product_heap = MulBinomialTournament<Number>();
            Bits<Number> encrypted_one_digits = encrypted_one.template to_digits<Bits<Number>>();
            Bits<Number> x_digits = x.template to_digits<Bits<Number>>();
            Bits<Number> y_digits = y.template to_digits<Bits<Number>>();

            for(int i = 0; i < encrypted_one_digits.get_bit_length(); ++i) {
                Number sum(x_digits[i] + y_digits[i]);
                Number result_at_i(encrypted_one_digits[i] - sum);
                product_heap.add_to_tournament(result_at_i, 0);
            }
            result = product_heap.unite_all(MulBinomialTournament<Number>::mul);
            return result;
        }
    }

    static VectorXi encryptVectorXi(VectorXi plainVector);

    static Number* encryptArray(Number* plainVector);

    static Number test();


};
#endif //SECURE_REPORT_FHEUTILS_H
