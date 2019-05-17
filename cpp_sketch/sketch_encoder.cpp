#include "sketch_encoder.h"


sketch_encoder::sketch_encoder(int n, int d): _n(n), _d(d) {
    _L = int(std::ceil(std::log2(n)));
    _h = _L * int(std::ceil(Ce * _d));
    _m = int(std::floor(_h / _d));
    _U = signature_matrix();
    _H = incidence_matrix();
    sketch = tensor_product(_H, _U);
}

VectorXi sketch_encoder::encode(VectorXi vector) {
    /*
    input: binary vector x with d 1s
    output: binary vector
    description: returns y = Mx
    running time: O(n+d^3*logn)
*/
    VectorXi result = VectorXi::Zero(sketch.rows());

    result = sketch * vector;

    for (int i = 0; i < result.size(); ++i) {
        (result[i] > 0) ? result[i] = 1 : result[i] = 0;
    }

    return result;
}

VectorXi sketch_encoder::decode(VectorXi vector) {
/*
    input: binary vector y=Mx
    output: binary vector
    description: extracts x from y
    running time: O(d^2*polylog(n))
    implement the psuedo code on page 17 on the paper on:
    https://pdfs.semanticscholar.org/c051/52418cc0a666eaa333efc119ea3c639279b1.pdf
 */
    int count = 0;
    VectorXi vector_tag;
    std::vector<int> result_singletons = find_singletons(vector, &vector_tag, &count);

    if(result_singletons.size() == _d) {
        VectorXi eigen_result_singleton = VectorXi::Map(result_singletons.data(), result_singletons.size());
        return eigen_result_singleton;
    }

    VectorXi result = find_doubletons(result_singletons, vector_tag, count);
    return result;
}

std::vector<int> sketch_encoder::find_singletons(VectorXi vector, VectorXi* vector_tag, int* count) {
    const int letter_size = _L;
    const int word_size = letter_size * 6;
    std::vector<int> G;
    std::vector<int> std_vector_tag;

    for (int i = 0; i < _h; ++i) {
        VectorXi vector1;
        vector1 = VectorXi::Zero(_L);
        vector1.block(0, 0, _L, 1) = vector.block(i*word_size, 0, _L, 1);

        VectorXi vector2;
        vector2 = VectorXi::Zero(_L);
        vector2.block(0, 0, _L, 1) = vector.block(i*word_size + letter_size, 0, _L, 1);

        if((hamming_weight(vector1) + hamming_weight(vector2)) == _L) {
            int vector1_int = binary_to_number(vector1);
            *count = *count + 1;

            if(std::find(G.begin(), G.end(), vector1_int) == G.end()) {
                G.push_back(vector1_int);
            }
        }
        else {
            std::vector<int> sub_vector_tag;
            for (int j = 0; j < word_size; ++j) {
                sub_vector_tag.push_back(vector(word_size*i+j));
            }
            std_vector_tag.insert( std_vector_tag.end(), sub_vector_tag.begin(), sub_vector_tag.end() );
        }
    }
    *vector_tag = VectorXi::Map(std_vector_tag.data(), std_vector_tag.size());

    return G;
}

VectorXi sketch_encoder::find_doubletons(std::vector<int> G, VectorXi vector_tag, int count) {
    VectorXi result;
    const int letter_size = _L;
    const int word_size = letter_size * 6;

    for (int i = 0; i < G.size(); ++i) {
        VectorXi U_g_l = _U.col(G[i]);
        VectorXi sig = U_g_l.replicate(_h - count, 1);

        VectorXi VTest = square_plus(vector_tag, sig);
        VectorXi mask(VTest.size());

        // Build mask
        for (int j = 0; j < _h - count; ++j) {
            mask.block(i * word_size + letter_size, 0, _L, 1) = VTest.block(i * word_size, 0, _L, 1);
            mask.block(i * word_size, 0, _L, 1) = VTest.block(i * word_size + letter_size, 0, _L, 1);

            mask.block(i * word_size + 3 * letter_size, 0, _L, 1) = VTest.block(i * word_size + 2 * letter_size, 0, _L, 1);
            mask.block(i * word_size + 2 * letter_size, 0, _L, 1) = VTest.block(i * word_size + 3 * letter_size, 0, _L, 1);

            mask.block(i * word_size + 5 * letter_size, 0, _L, 1) = VTest.block(i * word_size + 4 * letter_size, 0, _L, 1);
            mask.block(i * word_size + 4 * letter_size, 0, _L, 1) = VTest.block(i * word_size + 5 * letter_size, 0, _L, 1);
        }

        VTest = line_plus(VTest, mask);

        for (int j = 0; j < _h - count; ++j) {
            VectorXi vt_0(_L), vt_1(_L), vt_2(_L), vt_3(_L), vt_4(_L), vt_5(_L);

            vt_0.block(0, 0, _L, 1) = VTest.block(i * word_size, 0, _L, 1);
            vt_1.block(0, 0, _L, 1) = VTest.block(i * word_size + letter_size, 0, _L, 1);
            vt_2.block(0, 0, _L, 1) = VTest.block(i * word_size + 2 * letter_size, 0, _L, 1);
            vt_3.block(0, 0, _L, 1) = VTest.block(i * word_size + 3 * letter_size, 0, _L, 1);
            vt_4.block(0, 0, _L, 1) = VTest.block(i * word_size + 4 * letter_size, 0, _L, 1);
            vt_5.block(0, 0, _L, 1) = VTest.block(i * word_size + 5 * letter_size, 0, _L, 1);

            if ((hamming_weight(vt_0) + hamming_weight(vt_1)) == _L &&
                    (hamming_weight(vt_2) + hamming_weight(vt_3)) == _L &&
                    (hamming_weight(vt_4) + hamming_weight(vt_5)) == _L) {
                int ind1 = binary_to_number(vt_0);
                int ind3 = binary_to_number(vt_2);
                int ind5 = binary_to_number(vt_4);

                if ((_pi3_inv[ind3] == ind1) && (_pi5_inv[ind5] == ind1) &&
                    std::find(G.begin(), G.end(), ind1) == G.end()) {
                    G.push_back(ind1);
                }
            }
        }
    }
    result = VectorXi::Map(G.data(), G.size());
    return result;
}

VectorXi sketch_encoder::number_to_binary(int value) {
/*
    input: int i, int n
    output: array of size log(n)
    description: returns the binary representation of i with padding to n
    running time: O(log(n))
*/
    int int_value_current = value;
    int vector_size = int(std::ceil(std::log2(_n)));
    VectorXi binary_value_vector(vector_size);

    for(int array_index = vector_size-1; array_index >= 0; --array_index) {
        binary_value_vector(array_index) = (int_value_current % 2 == 0 ? 0 : 1);
        int_value_current /= 2;
    }

    return binary_value_vector;
}

int sketch_encoder::binary_to_number(VectorXi vector) {
    // base10
/*
    input: binary array
    output: int
    description: returns the decimal value the array represents
    running time: O(|b_list|)
*/
    int value = 0;
    long vector_size = vector.size();

    for(int i = vector_size-1, power = 1; i >= 0; --i, power *= 2) {
        value += vector(i) * power;
    }

    return value;
}

int sketch_encoder::hamming_weight(VectorXi vector) {
// Returns the hamming weight of vector
/*
    input: binary vector v
    output: int
    description: returns the hamming weight of v
    running time: O(|v|)
 */
    return vector.sum();
}

VectorXi sketch_encoder::square_plus(VectorXi  vector_a, VectorXi  vector_b) {
/*
    input: 2 binary vectors a,b
    output: vector over {0,1,?} when ? is 2 or 3
    description: returns b if a=0 or unknown if a=1
    running time: O(|a|)
 */
    return vector_a + 2 * vector_b;
}

VectorXi sketch_encoder::line_plus(VectorXi vector_a, VectorXi vector_b) {
/*
    input: 2 vectors a,b over {0,1,?} when ? is 2 or 3
    output: binary vector
    description: returns ~a if b is unknown or b otherwise
    running time: O(|a|)
*/
    VectorXi result(vector_a.size());
    for(int i = 0; i < vector_a.size(); ++i) {
        if(vector_a(i) >= 2) {
            result(i) = ((vector_b(i) == 0)  ? 1 : 0);
        }
        else {
            result(i) = vector_a(i);
        }
    }
    return result;
}

MatrixXi sketch_encoder::tensor_product(MatrixXi matrix_a, MatrixXi matrix_b) {
/*
    input: h by n incidence matrix H of type np.matrix, k by n signature matrix U of type np.matrix
    output: h*k by n d-disjunct matrix of type np.matrix
    description: return the tensor product of H and U
    running time: O(hkn)
 */
    MatrixXi result(matrix_a.rows() * matrix_b.rows(), matrix_a.cols());
    for (int j = 0; j < matrix_a.cols(); ++j) {
        MatrixXi result_column_j(matrix_b.rows(), 1);
        for (int i = 0; i < matrix_a.rows(); ++i) {
            MatrixXi value = (matrix_a(i, j) * matrix_b.col(j));

            result.block(i*value.rows(), j, value .rows(), 1) =
                    value.block(0, 0, value.rows(), 1);
        }
    }
    return result;
}

MatrixXi sketch_encoder::signature_matrix() {
/*
    input: int n
    output: 6log(n) by n signature matrix of type np.array
    description: return the tensor product of H and U
    running time: O(nlogn)
 */
    VectorXi perm_1 = generate_permutation(_n);
    VectorXi perm_2 = generate_permutation(_n);
    VectorXi perm_inv_1 = sort_indirect(perm_1);
    VectorXi perm_inv_2 = sort_indirect(perm_2);
    _pi3_inv = perm_inv_1;
    _pi5_inv = perm_inv_2;

    int binary_word = _L;

    MatrixXi result(6 * binary_word, _n);
    for (int i = 0; i < _n; ++i) {
        int sub_column_index = 0;
        VectorXi index_binary = number_to_binary(i);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = index_binary.block(0, 0, binary_word, 1);

        sub_column_index += 1;
        VectorXi index_binary_not = bitwise_not(index_binary);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = index_binary_not.block(0, 0, binary_word, 1);

        sub_column_index += 1;
        VectorXi perm_1_binary = number_to_binary(perm_1[i]);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = perm_1_binary.block(0, 0, binary_word, 1);

        sub_column_index += 1;
        VectorXi perm_1_binary_not = bitwise_not(perm_1_binary);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = perm_1_binary_not.block(0, 0, binary_word, 1);

        sub_column_index += 1;
        VectorXi perm_2_binary = number_to_binary(perm_2[i]);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = perm_2_binary.block(0, 0, binary_word, 1);

        sub_column_index += 1;
        VectorXi perm_2_binary_not = bitwise_not(perm_2_binary);
        result.block(sub_column_index * binary_word, i, binary_word, 1)
                = perm_2_binary_not.block(0, 0, binary_word, 1);
    }

    return result;
}

MatrixXi sketch_encoder::incidence_matrix() {
/*
    input: none
    output: h by n matrix of type np.array
    description: return the incidence matrix of a left regular bipartite graph
    running time: O(hn)
 */
    MatrixXi result(_n, _h);
    for (int i = 0; i < _h; ++i) {
        VectorXi column_i = generate_binary_vector(_n, _m);

        result.block(0, i, column_i.rows(), 1) = column_i.block(0, 0, column_i.rows(), 1);
    }

    return result.transpose();
}

VectorXi sketch_encoder::generate_binary_vector(int n, int d) {
    int p = n;
    int t = d;
    VectorXi result(p);
    result = VectorXi::Zero(p);

    //Get a random position in the 1xp matrix from 0-p
    for (int i = 0; i < t; ++i)
    {
        // Define random number generator
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dis(0, p-1);

        // Generate values until new unique index is generated
        int randPos = dis(mt) % p;
        while (result[randPos] == 1)
            randPos = dis(mt) % p;

        //Change the random position from a 0 to a 1
        result[randPos] = 1;
    }

    return result;
}

VectorXi sketch_encoder::generate_permutation(int n) {
    // Create vector from 0 to n-1
    MatrixXi vector = VectorXi::LinSpaced(n, 0, n-1);
    // Random generator
    std::random_device rd;
    std::mt19937 g(rd());

    // Permutation
    std::shuffle(vector.data(), vector.data() + vector.rows(), g);

    return vector;
}

VectorXi sketch_encoder::sort_indirect(VectorXi x) {
    /*
    Indirect sort is defined:
     Given array = permutation([0,n-1])
     for i in [0, n-1]:
        result(i) = index_of(i in array)
     */
    VectorXi result = VectorXi::LinSpaced(x.size(), 0, x.size()-1);

    std::sort(result.data(), result.data() + x.size(), [x](int i, int j){ return x[i] < x[j]; });

    return result;
}

VectorXi sketch_encoder::bitwise_not(VectorXi x) {
    VectorXi result(x.size());
    for (int i = 0; i < x.size(); ++i) {
        x[i] != 0 ? result[i] = 0 : result[i] = 1;
    }
    return result;
}

std::ostream& operator<< (std::ostream& stream, const sketch_encoder& matrix) {
    std::cout << "Ce: " << matrix.Ce << std::endl;
    std::cout << "_L: " << matrix._L << std::endl;
    std::cout << "_n: " << matrix._n << std::endl;
    std::cout << "_d: " << matrix._d << std::endl;
    std::cout << "_h: " << matrix._h << std::endl;
    std::cout << "_m: " << matrix._m << std::endl;

    // Matrices sizes
    std::cout << "_H: " << matrix._H.rows() << ", " << matrix._H.cols() << std::endl;
    std::cout << "_U: " << matrix._U.rows() << ", " << matrix._U.cols() << std::endl;
    std::cout << "_M: " << matrix.sketch.rows() << ", " << matrix.sketch.cols() << std::endl;
}
