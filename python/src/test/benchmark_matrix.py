import sys
import time
import csv
from pathlib import Path
from psutil import Process

from main.Matrix import MatrixMultiplier


def measure_memory_mb():
    return Process().memory_info().rss / 1048576


def execute_single_benchmark(matrix_size):
    a = MatrixMultiplier(matrix_size, random_seed=42).fill_random()
    b = MatrixMultiplier(matrix_size, random_seed=42).fill_random()
    mem_before = measure_memory_mb()
    start_time = time.time()
    product = a.multiply_with(b)
    end_time = time.time()
    mem_after = measure_memory_mb()

    return {
        'time': end_time - start_time,
        'memory': max(mem_before, mem_after)
    }


def run_benchmark_suite(sizes, repetitions=5):
    output_dir = Path(output_folder)
    output_dir.mkdir(exist_ok=True)
    csv_path = output_dir / "matrix_benchmark.csv"

    all_results = {size: [] for size in sizes}

    for size in sizes:
        print(f"Benchmarking {size}x{size} matrices...")

        for run_num in range(1, repetitions + 1):
            metrics = execute_single_benchmark(size)
            all_results[size].append(metrics)

    with open(csv_path, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Language', 'Matrix_Size', 'Time_ms', 'Memory_MB'])

        for size in sizes:
            metrics_list = all_results[size]
            avg_time_ms = (sum(m['time'] for m in metrics_list) / len(metrics_list)) * 1000
            avg_mem = sum(m['memory'] for m in metrics_list) / len(metrics_list)

            writer.writerow(['Python', size, round(avg_time_ms, 3), round(avg_mem, 2)])

    display_statistics(all_results)
    print(f"\nResults exported to: {csv_path}")


def display_statistics(results):
    print("\n" + "=" * 60)
    print("BENCHMARK STATISTICS")
    print("=" * 60)

    for size, metrics_list in results.items():
        avg_time = sum(m['time'] for m in metrics_list) / len(metrics_list)
        avg_mem = sum(m['memory'] for m in metrics_list) / len(metrics_list)

        print(f"Size {size:>4}: Time = {avg_time:.5f}s, Memory = {avg_mem:.2f} MB")


if __name__ == "__main__":
    matrix_sizes = [128, 256, 512, 1024]
    output_folder = sys.argv[1]
    run_benchmark_suite(matrix_sizes, repetitions=5)