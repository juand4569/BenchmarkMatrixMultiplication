#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

double** allocate_square_matrix(int dimension);
void deallocate_matrix(double** matrix, int dimension);
void fill_with_random_values(double** matrix, int dimension, unsigned int seed);
double** execute_matrix_product(double** left, double** right, int dimension);
int matrices_are_equal(double** first, double** second, int dimension, double tolerance);

#endif