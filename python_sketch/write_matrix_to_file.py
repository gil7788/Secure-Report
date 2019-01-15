#!/usr/bin/env python3

import numpy as np


def save_matrix_to_file(matrix, file_path):
    m = np.matrix(matrix)
    with open(file_path, 'wb') as f:
        for row in m:
            np.savetxt(f, row)


def file_test(file_name):
    try:
        a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
        a = a * 2
        save_matrix_to_file(a, file_name)
    except IOError as e:
        print("Couldn't open or write to file (%s)." % e)
