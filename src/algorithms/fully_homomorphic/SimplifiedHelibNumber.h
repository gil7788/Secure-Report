//
// Created by gil on 15.8.2020.
//

#ifndef SECURE_REPORT_SIMPLIFIEDHELIBNUMBER_H
#define SECURE_REPORT_SIMPLIFIEDHELIBNUMBER_H

#include <assert.h>
#include "SimplifiedHelibKeys.h"
#include "../../infrastructure/VectorUtils.h"
#include "EncryptedBits.h"
#include <helib_number.h>

class SimplifiedHelibNumber {
private:
    static SimplifiedHelibKeys *_prev_keys;
    SimplifiedHelibKeys *_keys;
    Ctxt _val;

    int _mul_depth;
    int _add_depth;

    long long power(long long base, long long exp) const {
        long long ret = 1;
        for (int i = 0; i < exp; ++i)
            ret *= base;
        return ret;
    }

    SimplifiedHelibNumber mult_by_recursive_adding(const SimplifiedHelibNumber &x, int e) {
        assert(e >= 0);
        if (e == 0)
            return SimplifiedHelibNumber(0);

        if (e == 2) {
            SimplifiedHelibNumber y = x + x;
            return y;
        }
        if (e == 1)
            return x;

        if (e & 1) {
            return x + mult_by_recursive_adding(x, e-1);
        } else {
            return mult_by_recursive_adding(x + x, e/2);
        }
    }

    void print(const char *s) {}
public:
    SimplifiedHelibNumber() : _keys(_prev_keys), _val(_prev_keys->publicKey()), _mul_depth(0), _add_depth(0) {
        print("allocating");
    }

    SimplifiedHelibNumber(long long v) : _keys(_prev_keys), _val(_keys->publicKey()), _mul_depth(0), _add_depth(0) { _keys->encrypt(_val, std::vector<long>(simd_factor(), v)); print("allocating"); }
    SimplifiedHelibNumber(const std::vector<long> &v) : _keys(_prev_keys), _val(_keys->publicKey()), _mul_depth(0), _add_depth(0) {
        _keys->encrypt(_val, v);
        print("allocating");
    }

    SimplifiedHelibNumber(const SimplifiedHelibNumber &n) : _keys(n._keys), _val(n._val), _mul_depth(n._mul_depth), _add_depth(n._add_depth) {
        print("allocating");
    }

    SimplifiedHelibNumber(const Ctxt &n) : _keys(_prev_keys), _val(n), _mul_depth(0), _add_depth(0) {
        print("allocating");
    }

    ~SimplifiedHelibNumber() {print("deleting"); }

    static int global_p() { return _prev_keys->p(); }
    static void set_global_keys(SimplifiedHelibKeys *k) { _prev_keys = k; }
    int in_range(int a) const { while (a < 0) a += _keys->p(); return a % _keys->p(); }
    static int static_in_range(int a) { while (a < 0) a += _prev_keys->p(); return a % _prev_keys->p(); }
    long to_int() const {
        return _keys->decrypt(_val);
    }

    std::vector<long int> to_vector() const { std::vector<long int> ret; _keys->decrypt(ret, _val); return ret; }
    SimplifiedHelibNumber from_int(int i) const {
        SimplifiedHelibNumber ret;
        _keys->encrypt(ret._val, i);
        return ret;
    }

    static SimplifiedHelibNumber static_from_int(int i) {
        SimplifiedHelibNumber ret;
        _prev_keys->encrypt(ret._val, i);
        return ret;
    }

    static unsigned int simd_factor() { return _prev_keys->simd_factor(); }
    int get_ring_size() const { return power(_keys->p(), _keys->r()); }
    int p() const { return _keys->p(); }
    int r() const { return _keys->r(); }
    void assert_co_prime(int) const {}

    SimplifiedHelibNumber &operator=(const SimplifiedHelibNumber &b) {
        _keys = b._keys;
        _val = b._val;
        _mul_depth = b._mul_depth;
        _add_depth = b._add_depth;
        return *this;
    }

    const Ctxt &v() const { return _val; }

    int add_depth() const { return _add_depth; }
    int mul_depth() const { return _mul_depth; }

    void add_depth(int d) { _add_depth = d; }
    void mul_depth(int d) { _mul_depth = d; }

    void shift_right() { _val.divideByP(); }

    void negate() { _val.negate(); }

    SimplifiedHelibNumber operator!() const { SimplifiedHelibNumber zp(1); zp -= *this; return zp; }
    SimplifiedHelibNumber operator-() const { SimplifiedHelibNumber zp(*this); zp.negate(); return zp; }
    SimplifiedHelibNumber operator-(const SimplifiedHelibNumber &z) const { SimplifiedHelibNumber zp(*this); zp -= z; return zp; }
    SimplifiedHelibNumber operator+(const SimplifiedHelibNumber &z) const { SimplifiedHelibNumber zp(*this); zp += z; return zp; }
    SimplifiedHelibNumber operator*(const SimplifiedHelibNumber &z) const { SimplifiedHelibNumber zp(*this); zp *= z; return zp; }

    void operator-=(const SimplifiedHelibNumber &z) {
        assert(_keys == z._keys);
        _val -= z._val;
        _mul_depth = max(_mul_depth, z._mul_depth);
        _add_depth = max(_add_depth, z._add_depth) + 1;
    }

    void operator+=(const SimplifiedHelibNumber &z) {
        assert(_keys == z._keys);
        _val += z._val;

        _mul_depth = max(_mul_depth, z._mul_depth);
        _add_depth = max(_add_depth, z._add_depth) + 1;
    }

    void operator*=(const SimplifiedHelibNumber &z) {
        assert(_keys == z._keys);
        _val.multiplyBy(z._val);
        _mul_depth = max(_mul_depth, z._mul_depth) + 1;
        _add_depth = max(_add_depth, z._add_depth);
    }

    SimplifiedHelibNumber operator-(int z) const { SimplifiedHelibNumber zp(*this); zp -= z; return zp; }
    SimplifiedHelibNumber operator+(int z) const { SimplifiedHelibNumber zp(*this); zp += z; return zp; }
    SimplifiedHelibNumber operator*(int z) const { SimplifiedHelibNumber zp(*this); zp *= z; return zp; }

    void operator-=(int z) { operator-=(SimplifiedHelibNumber(z)); }
    void operator+=(int z) { operator+=(SimplifiedHelibNumber(z)); }
    void operator*=(int z) { *this = mult_by_recursive_adding(*this, z); }

    int to_int_debug() {
        return 0;
    }

    template<class BITS>
    BITS to_digits() const {
        assert(p() == 2);
        assert(r() > 1);

        SimplifiedHelibNumber n = *this;
        BITS ret;
        ret.set_bit_length(r());

        for (int i = 0; i < r(); ++i) {
            std::vector<Ctxt> bits;
            extractDigits(bits, n._val, 0, false);

            SimplifiedHelibNumber bit(bits[0]);
            bit.mul_depth(mul_depth());
            bit.add_depth(add_depth());
            ret.set_bit(i, bit);

            if (i != r() - 1) {
                n -= bit;
                SimplifiedHelibNumber half_n = n;
                half_n._val.divideByP();
                n -= half_n;
            }
        }
        return ret;
    }

    SimplifiedHelibNumber rotate_left(int step) {
        SimplifiedHelibNumber ret(*this);
        _keys->rotate(ret._val, step);
        return ret;
    }

    void reduceNoiseLevel() {
        _val.modDownToLevel(_val.findBaseLevel());
    }

    friend std::ostream &operator<<(std::ostream &out, const SimplifiedHelibNumber &z);
    friend std::istream &operator>>(std::istream &in, SimplifiedHelibNumber &z);
};

inline std::ostream &operator<<(std::ostream &out, const SimplifiedHelibNumber &z) {
    out << z._val << " ";

    out
            << z._mul_depth << " "
            << z._add_depth << " ";

    return out;
}

inline std::istream &operator>>(std::istream &in, SimplifiedHelibNumber &z) {
    in >> z._val;

    in
            >> z._mul_depth
            >> z._add_depth;

    return in;
}

#endif //SECURE_REPORT_SIMPLIFIEDHELIBNUMBER_H
