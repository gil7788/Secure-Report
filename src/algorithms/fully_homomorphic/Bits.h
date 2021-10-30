//
// Created by gil on 15.8.2020.
//

#ifndef SECURE_REPORT_ENCRYPTEDBITS_H
#define SECURE_REPORT_ENCRYPTEDBITS_H

#include <iostream>
#include <vector>

using namespace std;


/**
 * Convert Fully Homomorphic bit vector to Fully Homomorphic number.
 * @tparam Number Plain/Encrypted Fully Homomorphic number template.
 */
template<class Number>
class Bits {
private:
    int _bit_length;
    vector<Number> _bits;

public:
    void set_bit_length(int bit_length) {
        _bit_length = bit_length;
    }

    int size() {
        return _bit_length;
    }

    Number operator[](int index) {
        return _bits[index];
    }

    int get_bit_length() {
        return _bit_length;
    }

    void set_bit(int i, Number& bit) {
        auto iterator = _bits.begin() + i;
        _bits.insert(iterator, bit);
    }

    vector<Number> get_bits() {
        return _bits;
    }

    Number to_number() {
        Number result{_bits};
        return result;
    }
};

#endif //SECURE_REPORT_ENCRYPTEDBITS_H
