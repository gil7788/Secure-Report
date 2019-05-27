#ifndef SECURE_REPORT_FHE_DB_H
#define SECURE_REPORT_FHE_DB_H

#include "PlainDatabase.h"
#include "SketchEncoder.h"
#include "VectorUtils.h"

/*
 * TODO
 * 1. Split test flow into functions - Done
 * 2. Add real queries - read and understand max's email about fhe_is_equal function
 * 3. Integrate real db
 * 4. Build db that contains costume type of objects (i.e. structs, classes)
 * */

template<class Number>
class FHEDatabase {
private:
    int _d;
    int _n;
public:
    SketchEncoder _encoder;

public:
    FHEDatabase(int db_size, int sparsity): _n(db_size),
                                            _d(sparsity),
                                            _encoder(db_size, sparsity) {}

    VectorXi decode(VectorXi encoded) {
        for (int i = 0; i < encoded.size(); ++i) {
            (encoded[i] > 0) ? encoded[i] = 1 : encoded[i] = 0;
        }
        VectorXi decoded = _encoder.decode(encoded);
        return decoded;
    }

    VectorXi fhe_decrypt(Number* sketchEnc) {
        MatrixXi sketch = _encoder.get_sketch();
        VectorXi encoded(sketch.rows());
        for (int i_output = 0; i_output < sketch.rows(); ++i_output) {
            encoded[i_output] = sketchEnc[i_output].to_int();
        }

        return encoded;
    }

    inline Number* fhe_encode(VectorXi plain_vector) {
        /* Description:
         * Build sketch vector of matched indices
         * */
        /*TODO
         * 1. Validate plain vector size, should be _n(db_size, as it plain_vector represents the db)
         * */
        MatrixXi sketch = _encoder.get_sketch();
        Number* out = new Number[sketch.rows()];
        for (int i_out = 0; i_out < sketch.rows(); ++i_out)
            out[i_out] = Number::static_from_int(0);

        for (int i_input = 0; i_input < plain_vector.size(); ++i_input) {
            Number x = Number::static_from_int(plain_vector[i_input]);

            for (int i_out = 0; i_out < sketch.rows(); ++i_out) {
                if (sketch(i_out, i_input) == 1)
                    out[i_out] += x;
            }
        }
        return out;
    }

// Test functions
public:
    bool test_sketch() {
        VectorXi plain_vector = VectorUtils::generate_binary_vector(_n, _d);
        MatrixXi sketch = _encoder.get_sketch();

        // Save matched indices for to indicate success/failure
        std::vector<int> indices = VectorUtils::getMatches(plain_vector);

        // Encode encrypted data
        Number* sketchEnc = fhe_encode(plain_vector);

        // Decrypt sketchEnc (which is encrypted encoding)
        VectorXi encoded = fhe_decrypt(sketchEnc);

        // Decode encoded into decoded
        VectorXi decoded = decode(encoded);

        // Print input and result
        std::cout << "Input indices: " << std::endl;
        for (auto i = indices.begin(); i != indices.end(); ++i)
            std::cout << *i << ' ';
        std::cout << std::endl;
        std::cout << "Decoded: " << decoded << std::endl;

        // Test if result is valid
        std::vector<int> decoded_std(decoded.data(), decoded.data() + decoded.rows() * decoded.cols());
        bool is_successful_decode = std::is_permutation(indices.begin(), indices.end(), decoded_std.begin());

        delete [] sketchEnc;
        return is_successful_decode;
    }
};


#endif
