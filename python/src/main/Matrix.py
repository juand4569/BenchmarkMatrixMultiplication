import random

class BasicMatrixMultiplier:
    def __init__(self):
        self.random = random.Random()

    def generate_matrices(self, n):
        A = [[self.random.random() for _ in range(n)] for _ in range(n)]
        B = [[self.random.random() for _ in range(n)] for _ in range(n)]
        return A, B

    def multiply(self, first_matrix, second_matrix):
        size = len(first_matrix)
        result_matrix = [[0.0 for _ in range(size)] for _ in range(size)]

        for i in range(size):
            for j in range(size):
                for k in range(size):
                    result_matrix[i][j] += first_matrix[i][k] * second_matrix[k][j]
        return result_matrix