import time
import math
import numpy as np
import unireedsolomon as rs


def decode(M, r):
    # input: t by n binary matrix M of type np.matrix, vector r with length t of type np.array
    # output: binary vector x of type np.array
    # description: decodes x out of M and r where r=M*x.
    # running time: O(nt)
    return np.prod(r | ~M.T, axis=1)


def decode_indices(M, r):
    # input: t by n binary matrix M of type np.matrix, vector r with length t of type np.array
    # output: binary vector x of type np.array
    # description: decodes x out of M and r where r=M*x.
    # running time: O(nt)
    return np.where(decode(M, r))[0]


def concatenateCodes(Cout, Cin):
    # input: m by n matrix over GF(q) Cout of type np.array, r by q binary matrix Cin of type np.array
    # output: r*m by n binary matrix of type np.array
    # description: generates a matrix represents Cout o Cin (concatenates the codes Cout and Cin)
    # running time: O(mnr)
    return np.concatenate([Cin[:, Cout[i]] for i in range(len(Cout))])


def createRScode(q, k):
    # input: q=2^i for some i of type int, k <= q of type int
    # output: q-1 by q^k matrix of type np.array
    # description: generates a matrix represents all reed solomon code words as columns
    # running time: O(c*k*q^k) where c is the encoding time
    m = np.math.log(q, 2)
    coder = rs.RSCoder(q - 1, k, c_exp=m, prim=rs.find_prime_polynomials(c_exp=m, single=True, generator=2),
                       generator=2)
    return np.array(
        [map(ord, list(coder.encode([i / (q ** j) % q for j in range(k - 1, -1, -1)]))) for i in range(q ** k)]).T


def generateDeterminsticMatrix(q, k):
    # input: q=2^i for some i of type int, k <= q of type int
    # output: O(q^2) by q^k matrix of type np.array
    # description: creates a d-disjunct matrix where d ~ q/k
    # running time: O(q^(k+2))
    Cout = createRScode(q, k)
    Cin = np.flip(np.identity(q, dtype=bool), 0)
    return concatenateCodes(Cout, Cin)


def generateRandomMatrix(t, n, d):
    # input: t,n dimensions and d value of the desired d-disjunct matrix of type int
    # output: t by m matrix M of type np.matrix
    # description: generates a t by n randomized potentially d-disjunct matrix
    # running time: O(nt)
    return ~np.random.randint(0, d, (t, n)).astype(dtype=bool)


def generateVector(n, d, indices=None):
    # input: n,d of type int
    # output: vector x of size n of type np.array
    # description: generates a vector of size n with d ones (Trues)
    # running time: O(n)
    x = np.zeros(n, dtype=bool)
    if indices is None:
        indices = np.random.choice(int(n), int(d), False)
    x[indices] = np.ones(int(d), dtype=bool)  # replaces d indices from False to True (0 to 1)
    return x
