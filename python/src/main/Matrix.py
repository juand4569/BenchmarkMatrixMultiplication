from random import Random


class MatrixMultiplier:
    def __init__(self, size, random_seed=None):
        self.size = size
        self.data = []
        self._rng = Random(random_seed)

    def fill_random(self):
        self.data = [[self._rng.random() for _ in range(self.size)]
                     for _ in range(self.size)]
        return self

    def multiply_with(self, other):
        if self.size != other.size:
            raise ValueError("Matrix dimensions must match")

        result = MatrixMultiplier(self.size)
        result.data = [[0.0] * self.size for _ in range(self.size)]

        for i in range(self.size):
            for j in range(self.size):
                for k in range(self.size):
                    result.data[i][j] += self.data[i][k] * other.data[k][j]

        return result