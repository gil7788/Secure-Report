import numpy as np
from disjunct import generateVector
import time

"""
M: d-disjunct matrix
U: signature matrix
d: number of defectives items
h: number of right nodes- number of bundles of tests
k: number of rows on U
n: number of items
L: log (base 2) of n
x: vector of set of items
y: M*x
H: incidence matrix, used to build M
m: number of 1's on each row of H
G: set of indices after decode
Ce: used to determine the success of rate of the decoding
"""


class Saffron:
    def __init__(self, n, Ce, d=2):
        # initializing the algorithm's parameters, as written above/in the documentation/in the paper
        self.n = n
        self.d = d
        self.L = int(np.ceil(np.log2(n)))
        self.h = self.L * int(np.ceil(Ce * self.d))
        self.pi3_inv = self.pi5_inv = None
        self.U = self.signature_matrix()
        m = self.h / self.d
        H = self.incidence_matrix(m)
        self.M = self.tensor_product(H, self.U)

    def b(self, i):
        # input: int i, int n
        # output: array of size log(n)
        # description: returns the binary representation of i with padding to n
        # running time: O(log(n))
        b = np.zeros(int(np.ceil(np.log2(self.n))), dtype=bool)
        j = 1
        while i > 0:
            b[-j] = i % 2
            i = i // 2
            j += 1
        return b

    @staticmethod
    def hw(v):
        # input: binary vector v
        # output: int
        # description: returns the hamming weight of v
        # running time: O(|v|)
        return np.sum(1 * v)

    def get_sketch_matrix(self):
        # returns <class 'numpy.ndarray'>
        return self.M

    @staticmethod
    def square_plus(b, a):
        # input: 2 binary vectors a,b
        # output: vector over {0,1,?} when ? is 2 or 3
        # description: returns b if a=0 or unknown if a=1
        # running time: O(|a|)
        return 2 * a + 1 * b

    @staticmethod
    def line_plus(b, a):
        # input: 2 vectors a,b over {0,1,?} when ? is 2 or 3
        # output: binary vector
        # description: returns ~a if b is unknown or b otherwise
        # running time: O(|a|)
        return np.array([not a[i] if b[i] >= 2 else b[i] for i in range(len(a))])

    @staticmethod
    def base10(b_list):
        # input: binary array
        # output: int
        # description: returns the decimal value the array represents
        # running time: O(|b_list|)
        return int(''.join(map(str, 1 * b_list)), 2)

    def signature_matrix(self):
        # input: int n
        # output: 6log(n) by n signature matrix of type np.array
        # description: return the tensor product of H and U
        # running time: O(nlogn)
        pi3 = np.random.permutation(self.n)
        self.pi3_inv = np.argsort(pi3)
        pi5 = np.random.permutation(self.n)
        self.pi5_inv = np.argsort(pi5)
        return np.concatenate([np.concatenate(
            [self.b(i), ~self.b(i), self.b(pi3[i]), ~self.b(pi3[i]), self.b(pi5[i]), ~self.b(pi5[i])])[np.newaxis].T for
                               i in range(self.n)], axis=1)

    def incidence_matrix(self, m):
        # input: none
        # output: h by n matrix of type np.array
        # description: return the incidence matrix of a left regular bipartite graph
        # running time: O(hn)
        return np.concatenate([generateVector(self.n, m)[np.newaxis] for i in range(self.h)], axis=0)

    @staticmethod
    def tensor_product(H, U):
        # input: h by n incidence matrix H of type np.matrix, k by n signature matrix U of type np.matrix
        # output: h*k by n d-disjunct matrix of type np.matrix
        # description: return the tensor product of H and U
        # running time: O(hkn)
        columns = [np.concatenate([H[i, j] * U[:, j] for i in range(len(H))])[np.newaxis].T for j in range(len(H[0]))]
        return np.concatenate(columns, axis=1)

    def encode(self, x):
        # input: binary vector x with d 1s
        # output: binary vector
        # description: returns y = Mx
        # running time: O(n+d^3*logn)
        return np.sum(self.M.T[np.where(x)[0]].T, axis=1, dtype=bool)

    def decode(self, y):
        # input: binary vector y=Mx
        # output: binary vector
        # description: extracts x from y
        # running time: O(d^2*polylog(n))
        # implement the psuedo code on page 17 on the paper on:
        # https://pdfs.semanticscholar.org/c051/52418cc0a666eaa333efc119ea3c639279b1.pdf
        G = []
        count = 0
        y_tag = np.array([], dtype=bool)
        L = self.L
        y = y.reshape(self.h, 6, L)
        for i in range(self.h):  # finding singletons
            y1 = y[i][0]
            y2 = y[i][1]

            if self.hw(y1) + self.hw(y2) == L:
                g = self.base10(y1)
                count += 1
                if g not in G:
                    G.append(g)
            else:
                y_tag = np.concatenate((y_tag, y[i].reshape(6 * L)))
        if len(G) == self.d:  # if x was found with singletons only there is no need to look for doubletons
            return G
        for l in range(len(G)):  # finding remaining recoverable doubletons
            U_g_l = self.U[:, G[l]]
            sig = np.tile(U_g_l, self.h - count)
            VTest = self.square_plus(y_tag, sig).reshape(self.h - count, 6, L)

            mask = np.concatenate([np.concatenate((VTest[s][1], VTest[s][0],
                                                   VTest[s][3], VTest[s][2],
                                                   VTest[s][5], VTest[s][4]))
                                   for s in range(self.h - count)])

            VTest = VTest.reshape((self.h - count) * 6 * L)
            VTest = self.line_plus(VTest, mask)
            VTest = VTest.reshape(self.h - count, 6, L)
            for i in range(self.h - count):
                vt = VTest[i]
                if self.hw(vt[0]) + self.hw(vt[1]) == L and self.hw(vt[2]) + self.hw(vt[3]) == L and self.hw(
                        vt[4]) + self.hw(vt[5]) == L:
                    ind1 = self.base10(vt[0])
                    ind3 = self.base10(vt[2])
                    ind5 = self.base10(vt[4])
                    if self.pi3_inv[ind3] == ind1 and self.pi5_inv[ind5] == ind1 and (ind1 not in G):
                        G.append(ind1)
        return G


if __name__ == '__main__':

    for j in range(9, 10):
        n = 2 ** j
        d = 2
        tries = 1000
        Ce = 11.36
        t0 = time.time()
        saff = Saffron(n, Ce, d)
        saff.get_sketch_matrix()
        t = time.time() - t0
        cc = 0
        runtime = 0
        for i in range(tries):
            if i % 100 == 0:
                saff = Saffron(n, Ce, d)
                print('-')
            indices = np.random.choice(n, d, False)
            import disjunct

            x = disjunct.generateVector(n, d, indices)
            y = saff.encode(x)
            t0 = time.time()  # timing the running time of the algorithm
            dec = saff.decode(y)
            t = time.time() - t0
            runtime += t
            if np.array_equal(np.sort(dec), np.sort(indices)):
                cc += 1
        print('\nn = ', n)
        print('\nd = ', d)
        print('succ', cc)
        print('runtime ', runtime)
