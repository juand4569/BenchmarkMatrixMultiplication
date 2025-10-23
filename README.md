# BenchmarkMatrixMultiplication
Comparative performance analysis of basic matrix multiplication algorithm (O(n³)) across Java, Python, and C implementations.

**Course:** Big Data  
**Institution:** Universidad de Las Palmas de Gran Canaria
**Date:** October 2024

## Test Environment

**Hardware:**
- CPU: AMD Ryzen 5 7520U with Radeon Graphics (2.80 GHz)
- RAM: 16 GB (15.2 GB usable)
- Storage: SSD

**Software:**
- OS: Microsoft Windows 11 Home (64-bit)
- Java: JDK 21.0.5 (OpenJDK)
- Python: 3.11
- C: Github's Codespace

## Repository Structure

```
BenchmarkMatrixMultiplication/
├── src/
│   ├── java/
│   │   ├── MatrixMultiplicationBenchmarking.java
│   │   └── Matrix.java
│   ├── python/
│   │   ├── Matrix.py
│   │   └── benchmark_matrix.py
│   └── c/
│       ├── matrix_operations.c
│       ├── matrix_operations.h
│       └── run_benchmark.c
├── results/
│   ├── benchmark_c.csv
│   ├── benchmark_java.csv
│   └── benchmark_py.csv
└── README.md
```

## Matrix Sizes Tested

- 128×128
- 256×256
- 512×512
- 1024×1024

## Execution Instructions

### Java (JMH Framework)
```bash
# Using IntelliJ IDEA JMH Plugin
# Run MatrixMultiplicationBenchmarking.java
# Configurations:
# - Mode: AverageTime
# - Warmup: 3 iterations, 1s each
# - Measurement: 5 iterations, 1s each
# - Fork: 1
```

Memory measurement implemented with OSHI library due to JMH limitations.

### Python
```bash
python matrix_multiply.py
```

Custom benchmarking with `time.perf_counter()` and `tracemalloc` for memory profiling.

### C
```bash
gcc src/main/matrix_operations.c src/test/run_benchmark.c -o benchmark -O3 -I src/main

./benchmark results
```
## Results Summary

### Execution Time (ms)

| Matrix Size | Java | Python | C |
|------------|------|--------|---|
| 128×128 | 1.62 | 165.95 | 2.41 |
| 256×256 | 18.57 | 1354.48 | 27.50 |
| 512×512 | 178.15 | 12426.36 | 175.19 |
| 1024×1024 | 4143.32 | 260247.13 | 1917.39 |

### Memory Usage (MB)

| Matrix Size | Java | Python | C |
|------------|------|--------|---|
| 128×128 | 187.44 | 88.82 | 9.87 |
| 256×256 | 195.11 | 93.72 | 9.87 |
| 512×512 | 219.19 | 116.49 | 9.87 |
| 1024×1024 | 251.36 | 208.77 | 25.74 |

### Key Findings

1. **Performance Ranking:** C ≈ Java >> Python
2. **Python is ~100x slower** than compiled languages for large matrices
3. **C uses 10-20x less memory** than Java/Python due to minimal runtime overhead
4. **All implementations scale as O(n³)** as expected

# Benchmark Tools Used

- **Java:** JMH (Java Microbenchmark Harness) + OSHI for memory monitoring
- **Python:** Custom benchmark with `time.time()` for timing and `psutil` for memory measurement
- **C:** Platform-specific timing (`QueryPerformanceCounter` on Windows, `gettimeofday` on Unix) and memory tracking (`GetProcessMemoryInfo` on Windows, `getrusage` on Unix)

## Limitations and Notes

- Java results extracted from JMH with OSHI thread-based memory sampling
- Python uses RSS (Resident Set Size) memory measurement via psutil
- C memory measurements are platform-dependent (RSS-based on Unix, Working Set on Windows)

## References

- JMH Documentation: https://openjdk.org/projects/code-tools/jmh/
- OSHI Library: https://github.com/oshi/oshi
- psutil Documentation: https://github.com/giampaolo/psutil
- Assignment Guidelines: BD Individual Assignments - ULPGC

## License

Academic project for Big Data course, ULPGC 2024-2025.