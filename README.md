# Secure-Report

## Dependencies:

1. pThread
2. GMP
3. NTL
4. FHE
5. Liphe
6. Google Tests (gtest)
7. Eigen3

* All dependencies include files should be at: "/usr/include" or "/usr/local/include" 
and all lib files should be at: "/usr/lib" "/usr/local/lib" Linux

## Saffron - SketchEncoder(.h/.cpp)
*Saffron* is an algorithm for summarizing large sparse vector into indices that represent "True" bit in the original vector.
Saffron is mainly based on sketch matrices.

### Theoretical Limitations of Saffron Algorithm
Saffron builds *d-disjunct matrix*, also known as *d-independent matrix* to preform d tests simultaneously 
at the same time (at the same request) in such way it will be able to decode up to d "True" indices in input binary vector.

Following few bounds on Saffron algorithm:

1.There is no strongly explicit construction for incidence matrices *H*. As a consequence, there is no
  strongly explicit construction for generating d-disjunct matrices.
   
2.False positives occur with probability O(C(epsilon)*d/n^2)

3.Saffron scheme identifies at least (1-epsilon)*d "True" indices with probability 1 − O(C(epsilon)*d/n^2)

4.Decoding complexity is: O(d*log(n)), where n is input vector size, and d is hamming_weight(input_vector)

5.epsilon->C(epsilon) mapping:
* 10^-3   <->   6.13
* 10^-4   <->   7.88
* 10^-5   <->   9.63
* 10^-6   <->   11.36
* 10^-7   <->   13.10
* 10^-8   <->   14.84
* 10^-9   <->   16.57
* 10^-10  <->   18.30

### Technical Limitation and Dependencies
1. Current C++ implementation use Eigen library for Linear Algebra, for more detail please visit: 
http://eigen.tuxfamily.org/index.php?title=Main_Page
2. For Python implementation you can check python_sketch directory in home directory of this repository, for wrapped and
 ready version for benchmarking visit: https://github.com/gil7788/disjunct_benchmark

### References
You can read full paper here: 
https://pdfs.semanticscholar.org/c051/52418cc0a666eaa333efc119ea3c639279b1.pdf