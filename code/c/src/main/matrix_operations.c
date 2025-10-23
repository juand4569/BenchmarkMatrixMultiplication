#include "matrix_operations.h"
#include <stdlib.h>
#include <math.h>

double** allocate_square_matrix(int dimension) {
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    for (int i = 0; i < dimension; i++) {
        matrix[i] = (double*)malloc(dimension * sizeof(double));
    }
    return matrix;
}

void deallocate_matrix(double** matrix, int dimension) {
    for (int i = 0; i < dimension; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_with_random_values(double** matrix, int dimension, unsigned int seed) {
    srand(seed);
    for (int row = 0; row < dimension; row++) {
        for (int col = 0; col < dimension; col++) {
            matrix[row][col] = (double)rand() / RAND_MAX;
        }
    }
}

double** execute_matrix_product(double** left, double** right, int dimension) {
    double** result = allocate_square_matrix(dimension);

    for (int row = 0; row < dimension; row++) {
        for (int col = 0; col < dimension; col++) {
            double sum_accumulator = 0.0;
            for (int idx = 0; idx < dimension; idx++) {
                sum_accumulator += left[row][idx] * right[idx][col];
            }
            result[row][col] = sum_accumulator;
        }
    }

    return result;
}

int matrices_are_equal(double** first, double** second, int dimension, double tolerance) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (fabs(first[i][j] - second[i][j]) > tolerance) {
                return 0;
            }
        }
    }
    return 1;
}