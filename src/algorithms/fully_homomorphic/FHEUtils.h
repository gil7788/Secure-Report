#ifndef SECURE_REPORT_FHEUTILS_H
#define SECURE_REPORT_FHEUTILS_H

#include <Eigen/Dense>
#include <helib_number.h>
#include <binomial_tournament.h>
#include "GenericZP.h"
#include "EncryptedBits.h"

using namespace std;
using Eigen::VectorXi;

/**
 * Contains static methods for evaluating operations on encrypted numbers, in Fully Homomorphic fashion.
 * @tparam Number Plain/Encrypted Fully Homomorphic number template.
 */
template<class Number>
class FHEUtils{
private:
    static Number areSuffixesEqual(Number& x, Number& y, int suffix_begining_index) {
        const int word_length = x.r();
        if(suffix_begining_index >= word_length) {
            Number result{1};
            return result;
        }

        auto vec = std::vector<long>(word_length, 1);
        auto one_vector = VectorUtils::std_vector_to_number(vec);
        Number encrypted_one(one_vector);

        if(encrypted_one.r() <= 1) {
            Number result(encrypted_one - x + y);
            return result;
        }
        else {
            MulBinomialTournament<Number> product_heap = MulBinomialTournament<Number>();
            Bits<Number> encrypted_one_digits = encrypted_one.template to_digits<Bits<Number>>();
            Bits<Number> x_digits = x.template to_digits<Bits<Number>>();
            Bits<Number> y_digits = y.template to_digits<Bits<Number>>();

            for(int i = suffix_begining_index; i < word_length; ++i) {
                Number result_at_i{encrypted_one_digits[i] - x_digits[i] + y_digits[i]};
                product_heap.add_to_tournament(result_at_i, 0);
            }
            Number result{product_heap.unite_all(MulBinomialTournament<Number>::mul)};
            return result;
        }
    }

public:
    /**
     * @param x y Fully Homomorphic numbers
     * @return x greater than or equals to y
     */
    static Number greaterThanOrEquals(Number& x, Number& y) {
        return lessThan(y, x) + areEqualBinary(x, y);
    }

    /**
     * @param x y Fully Homomorphic numbers
     * @return x less than or equals to y
     */
    static Number lessThanOrEquals(Number& x, Number& y) {
        return lessThan(x, y) + areEqualBinary(x, y);
    }

    /**
     * @param x y Fully Homomorphic numbers
     * @return x equals to y
     */
    static Number areEqualBinary(Number& x, Number& y) {
        return areSuffixesEqual(x, y, 0);
    }

    /**
     * @param x y Fully Homomorphic numbers
     * @return x greater than y
     */
    static Number greaterThan(Number& x, Number& y) {
        return lessThan(y, x);
    }

    /**
     * @param x y Fully Homomorphic numbers
     * @return x less than y
     */
    static Number lessThan(Number& x, Number& y) {
        const int word_length = x.r();
        auto vec = std::vector<long>(word_length, 1);
        auto one_vector = VectorUtils::std_vector_to_number(vec);
        Number encrypted_one(one_vector);

        if(encrypted_one.r() <= 1) {
            Number result{encrypted_one - x + y};
            return result;
        }
        else {
            AddBinomialTournament<Number> sum_heap;
            Bits<Number> encrypted_one_digits = encrypted_one.template to_digits<Bits<Number>>();
            Bits<Number> x_digits = x.template to_digits<Bits<Number>>();
            Bits<Number> y_digits = y.template to_digits<Bits<Number>>();

            for(int i = 0; i < word_length; ++i) {
                Number sum{(x_digits[i] + encrypted_one_digits[i])};
                Number step_i_accumulation = (y_digits[i] * sum) * areSuffixesEqual(x, y, i+1);
                sum_heap.add_to_tournament(step_i_accumulation, 0);
            }
            Number accumulated{sum_heap.unite_all(AddBinomialTournament<Number>::add)};
            return accumulated;
        }
    }

    // @TODO consider to delete, not implemented
    static VectorXi encryptVectorXi(VectorXi plainVector);

    static Number* encryptArray(Number* plainVector);

    static Number test();
};
#endif //SECURE_REPORT_FHEUTILS_H
