//
// Created by gil on 9.8.2020.
//

#ifndef SECURE_REPORT_GENERICZP_H
#define SECURE_REPORT_GENERICZP_H


#include <assert.h>
#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>
#include <functional>
#include "Config.h"
#include "utils/mathmatical/VectorUtils.h"

#include <stdio.h>

class GenericZP {
private:
    int _simd;
    static long long _prev_p;
    static long long _prev_r;
    long long _p;
    long long _r;
    std::vector<long> _val;

    int _mul_depth;
    int _add_depth;

public:
    GenericZP() : _simd(constants::SIMD), _p(_prev_p), _r(_prev_r), _mul_depth(0), _add_depth(0) {}
    GenericZP(const long v) : _simd(constants::SIMD), _p(_prev_p), _r(_prev_r), _mul_depth(0), _add_depth(0) {
        set_all(v);
    }

    GenericZP(const std::vector<long int> &v) : _simd(constants::SIMD), _p(_prev_p), _r(_prev_r), _mul_depth(0), _add_depth(0) {
        for (int i = 0; i < _simd; ++i) {
            _val.push_back(mod(v[i]));
        }
    }

    GenericZP(const long v, long long p) : _simd(constants::SIMD), _p(p), _r(_prev_r), _mul_depth(0), _add_depth(0) {
        set_all(v);
    }
    GenericZP(long v, long long p, long long r) : _simd(constants::SIMD), _p(p), _r(r), _mul_depth(0), _add_depth(0) {
        set_all(v);
    }
    GenericZP(const GenericZP &zp) : _simd(zp._simd), _p(zp._p), _r(zp._r), _mul_depth(zp._mul_depth), _add_depth(zp._add_depth) {
        for (int i = 0; i < _simd; ++i) {
            _val.push_back(mod(zp._val[i]));
        }
    }

    void set_all(const long a) {
        for (int i = 0; i < _simd; ++i) {
            _val.push_back(mod(a));
        }
    }

    static long long power(long long base, long long exp) {
        long long ret = 1;
        for (int i = 0; i < exp; ++i)
            ret *= base;
        return ret;
    }

    int in_range(int a) const {
        while (a < 0) {
            a += power(_p,_r);
        }
        return a % power(_p,_r);
    }

    static int static_in_range(int a) {
        while (a < 0) {
            a += power(_prev_p,_prev_r);
        }
        return a % power(_prev_p,_prev_r);
    }

    int to_int() const {
        return _val[0] % power(_p,_r);
    }

    std::vector<long> to_vector() const {
        return _val;
    }
    int to_int_debug() {
        return VectorUtils::std_vector_to_number(_val);
    }

    GenericZP from_int(int i) const {
        return GenericZP(i);
    }

    static GenericZP static_from_int(int i) {
        return GenericZP(i);
    }

    unsigned int simd_factor() {
        return _simd;
    }

    static void set_global_p(long long p, long long r = 1) {
        _prev_p = p; _prev_r = r;
    }

    static int global_p() {
        return _prev_p;
    }

    static int get_global_ring_size() {
        return power(_prev_p, _prev_r);
    }

    void set_p(long long p, long long r = 1) {
        _p = _prev_p = p;
        _r = _prev_r = r;
    }

    long long p() const {
        return _p;
    }

    long long r() const {
        return _r;
    }

    long long get_ring_size() const {
        return power(p(), r());
    }

    long long inv(long long x) const {
        assert(r() == 1);
        return (p() <= 3) ? x : power(x, p() - 2);
    }

    long long mod(long long x) const {
        return ((x % get_ring_size()) + get_ring_size()) % get_ring_size();
    }

    int add_depth() const {
        return _add_depth;
    }

    int mul_depth() const {
        return _mul_depth;
    }

    GenericZP &operator=(const GenericZP &b) {
        _p = b._p;
        _r = b._r;
        _mul_depth = b._mul_depth;
        _add_depth = b._add_depth;
        for (int i = 0; i < _simd; ++i)
            _val.push_back(b._val[i]);
        return *this;
    }

    void divide_by_p() {
        assert(_r > 1);
        for (int i = 0; i < _simd; ++i)
            _val[i] /= _p;
    }

    GenericZP rotate_left(int step) const {
        GenericZP ret(*this);

        for (int i = 0; i < _simd; ++i)
            ret._val[i] = _val[(i + step) % _simd];
        return ret;
    }

    GenericZP operator-() const {
        GenericZP zp(*this);
        for (int i = 0; i < _simd; ++i)
            zp._val[i] = -zp._val[i];
        return zp;
    }

    GenericZP operator-(const GenericZP &z) const {
        GenericZP zp(*this);
        zp -= z;
        return zp;
    }

    GenericZP operator+(const GenericZP &z) const {
        GenericZP zp(*this);
        zp += z;
        return zp;
    }

    GenericZP operator*(const GenericZP &z) const {
        GenericZP zp(*this);
        zp *= z;
        return zp;
    }

    GenericZP operator!() const {
        GenericZP zp(*this);
        zp = GenericZP(1) - zp;
        return zp;
    }

    void operator-=(const GenericZP &z) {
        assert(_p == z._p);
        assert(_r == z._r);
        for (int i = 0; i < _simd; ++i)
            _val[i] = mod(_val[i] - z._val[i]);
        _mul_depth = std::max(_mul_depth, z._mul_depth);
        _add_depth = std::max(_add_depth, z._add_depth) + 1;
    }

    void operator+=(const GenericZP &z) {
        assert(_p == z._p);
//        assert(_r == z._r);
        for (int i = 0; i < _simd; ++i)
            _val[i] = mod(_val[i] + z._val[i]);
        _mul_depth = std::max(_mul_depth, z._mul_depth);
        _add_depth = std::max(_add_depth, z._add_depth) + 1;
    }

    void operator*=(const GenericZP &z) {
        assert(_p == z._p);
//        assert(_r == z._r);
        for (int i = 0; i < _simd; ++i)
            _val[i] = mod(_val[i] * z._val[i]);
        _mul_depth = std::max(_mul_depth, z._mul_depth) + 1;
        _add_depth = std::max(_add_depth, z._add_depth);
    }

    void assert_co_prime(int a) const {
        assert(a != 0);
        assert(a != 1);

        for (int i = 0; i < _simd; ++i) {
            int b = _val[i];

            if (b > 1) {

                while (((b % a) != 0) && ((b % a) != 0)) {
                    if (a > b)
                        a -= b;
                    else
                        b -= a;
                }
                int gcd = (a < b) ? a : b;
                assert (gcd == 1);
            }
        }
    }

    void reduceNoiseLevel() {}

    friend std::ostream &operator<<(std::ostream &out, const GenericZP &z);

    friend std::istream &operator>>(std::istream &out, GenericZP &z);

    template<class BITS>
    BITS to_digits() const {
        BITS ret;
        ret.set_bit_length(r());
        for (int i = 0; i < r(); ++i) {
            GenericZP bit(0, p(),1);
            for (int s = 0; s < _simd; ++s) {
                auto value = (_val[s] / power(p(), i)) % p();
                bit._val[0] = value;
            }
            ret.set_bit(i, bit);
        }
        return ret;
    }
};

inline std::ostream &operator<<(std::ostream &out, const GenericZP &z) {
    out
            << z._p << " "
            << z._r << " ";

    out << z._simd << " ";
    for (int i = 0; i < z._simd; ++i)
        out << z._val[i] << " ";

    out
            << z._mul_depth << " "
            << z._add_depth << " ";

    return out;
}

inline std::istream &operator>>(std::istream &in, GenericZP &z) {
    in
            >> z._p
            >> z._r;

    int simd_size;
    in >> simd_size;

    for (int i = 0; i < z._simd; ++i) {
        int simd_value_i;
        in >> simd_value_i;
        z._val.push_back(simd_value_i);
    }

    in
            >> z._mul_depth
            >> z._add_depth;

    return in;
}

#endif //SECURE_REPORT_TEST_test_zp_H
