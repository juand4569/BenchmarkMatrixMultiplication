#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix_operations.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

double get_current_time() {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}

double get_memory_usage_mb() {
    PROCESS_MEMORY_COUNTERS mem_info;
    GetProcessMemoryInfo(GetCurrentProcess(), &mem_info, sizeof(mem_info));
    return (double)mem_info.WorkingSetSize / (1024.0 * 1024.0);
}

void make_directory(const char* path) {
    CreateDirectoryA(path, NULL);
}

#else
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

double get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

double get_memory_usage_mb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return (double)usage.ru_maxrss / 1024.0;
}

void make_directory(const char* path) {
    mkdir(path, 0777);
}
#endif

typedef struct {
    int size;
    int run_number;
    double time_seconds;
    double memory_mb;
} BenchmarkRecord;

void perform_benchmark_iteration(int matrix_size, int iteration, BenchmarkRecord* record) {
    double** matrix_a = allocate_square_matrix(matrix_size);
    double** matrix_b = allocate_square_matrix(matrix_size);

    fill_with_random_values(matrix_a, matrix_size, 42);
    fill_with_random_values(matrix_b, matrix_size, 43);

    double mem_before = get_memory_usage_mb();
    double time_start = get_current_time();

    double** result = execute_matrix_product(matrix_a, matrix_b, matrix_size);

    double time_end = get_current_time();
    double mem_after = get_memory_usage_mb();

    record->size = matrix_size;
    record->run_number = iteration;
    record->time_seconds = time_end - time_start;
    record->memory_mb = (mem_before > mem_after) ? mem_before : mem_after;

    deallocate_matrix(matrix_a, matrix_size);
    deallocate_matrix(matrix_b, matrix_size);
    deallocate_matrix(result, matrix_size);
}

void write_csv_header(FILE* file) {
    fprintf(file, "Language,Matrix_Size,Time_ms,Memory_MB\n");
}

void write_average_results_to_csv(FILE* file, BenchmarkRecord* records,
                                   int total_records, int* sizes, int num_sizes) {
    for (int s = 0; s < num_sizes; s++) {
        int current_size = sizes[s];
        double total_time = 0.0;
        double total_mem = 0.0;
        int count = 0;

        for (int i = 0; i < total_records; i++) {
            if (records[i].size == current_size) {
                total_time += records[i].time_seconds;
                total_mem += records[i].memory_mb;
                count++;
            }
        }

        double avg_time_ms = (total_time / count) * 1000.0;
        double avg_mem = total_mem / count;

        fprintf(file, "C,%d,%.3f,%.2f\n", current_size, avg_time_ms, avg_mem);
    }
}

void compute_and_display_averages(BenchmarkRecord* records, int total_records,
                                  int* sizes, int num_sizes, int runs_per_size) {
    printf("\n===== AVERAGE RESULTS =====\n");
    printf("%-12s %-18s %-20s\n", "Size", "Avg Time (s)", "Avg Memory (MB)");

    for (int s = 0; s < num_sizes; s++) {
        int current_size = sizes[s];
        double total_time = 0.0;
        double total_mem = 0.0;
        int count = 0;

        for (int i = 0; i < total_records; i++) {
            if (records[i].size == current_size) {
                total_time += records[i].time_seconds;
                total_mem += records[i].memory_mb;
                count++;
            }
        }

        printf("%-12d %-18.5f %-20.5f\n",
               current_size,
               total_time / count,
               total_mem / count);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <output_directory>\n", argv[0]);
        return 1;
    }

    make_directory(argv[1]);

    char csv_filename[512];
    #ifdef _WIN32
    snprintf(csv_filename, sizeof(csv_filename), "%s\\benchmark_c.csv", argv[1]);
    #else
    
    snprintf(csv_filename, sizeof(csv_filename), "%s/benchmark_c.csv", argv[1]);
    #endif

    int test_sizes[] = {128, 256, 512, 1024};
    int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);
    int iterations_per_size = 5;
    int total_benchmarks = num_sizes * iterations_per_size;

    BenchmarkRecord* all_records = (BenchmarkRecord*)malloc(
        total_benchmarks * sizeof(BenchmarkRecord));

    FILE* output_file = fopen(csv_filename, "w");
    if (!output_file) {
        printf("Error: Cannot open file %s\n", csv_filename);
        free(all_records);
        return 1;
    }

    write_csv_header(output_file);

    printf("Starting benchmark execution...\n");

    int record_index = 0;
    for (int s = 0; s < num_sizes; s++) {
        int size = test_sizes[s];
        for (int iter = 1; iter <= iterations_per_size; iter++) {
            BenchmarkRecord current_record;
            perform_benchmark_iteration(size, iter, &current_record);

            all_records[record_index++] = current_record;

            printf("Size %d | Iteration %d | Time: %.5fs\n",
                   size, iter, current_record.time_seconds);
        }
    }

    write_average_results_to_csv(output_file, all_records, total_benchmarks,
                                 test_sizes, num_sizes);

    fclose(output_file);

    compute_and_display_averages(all_records, total_benchmarks,
                                test_sizes, num_sizes, iterations_per_size);

    printf("\nBenchmark completed. Results saved at: %s\n", csv_filename);
    free(all_records);
    return 0;
}