/**
 * basicstats.c
 * 
 * This program calculates and prints statistical measures for a set of data
 * read from a file. The statistics calculated include mean, median, mode,
 * standard deviation, and harmonic mean. The program demonstrates dynamic
 * memory management, basic statistical computation without the use of the
 * math library, and custom implementation of the square root function.
 * 
 * Author: Jung Jun Kim
 * Date: 2024-04-17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
double* enlarge_array(double* old_array, int old_size, int* new_capacity);
double calculate_mean(const double* data, int size);
double calculate_stddev(const double* data, int size, double mean);
double calculate_median(double* data, int size);
double calculate_mode(double* data, int size);
double calculate_harmonic_mean(const double* data, int size);
double babylonian_sqrt(double value);
int compare_double(const void* a, const void* b);
void print_results(int size, int capacity, double mean, double median, double mode, double stddev, double harmonic_mean);

/**
 * Main function which handles file input and orchestrates the computation of statistics.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int size = 0, capacity = 20;
    double* data = (double*) malloc(capacity * sizeof(double));
    if (!data) {
        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        free(data);
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    double value;
    while (fscanf(file, "%lf", &value) == 1) {
        if (size == capacity) {
            double* new_data = enlarge_array(data, size, &capacity);
            if (!new_data) {
                free(data);
                fclose(file);
                return EXIT_FAILURE;
            }
            data = new_data;
        }
        data[size++] = value;
    }
    fclose(file);

    qsort(data, size, sizeof(double), compare_double);
    double mean = calculate_mean(data, size);
    double median = calculate_median(data, size);
    double mode = calculate_mode(data, size);
    double stddev = calculate_stddev(data, size, mean);
    double harmonic_mean = calculate_harmonic_mean(data, size);

    print_results(size, capacity, mean, median, mode, stddev, harmonic_mean);
    free(data);
    return EXIT_SUCCESS;
}

/**
 * Function to enlarge the existing data array when capacity is reached.
 * Allocates a new array twice the size of the current one, copies old data, and frees the old array.
 */
double* enlarge_array(double* old_array, int old_size, int* new_capacity) {
    *new_capacity = 2 * (*new_capacity);
    double* new_array = (double*) malloc(*new_capacity * sizeof(double));
    if (!new_array) {
        fprintf(stderr, "Memory allocation failed during enlargement.\n");
        return NULL;
    }
    memcpy(new_array, old_array, old_size * sizeof(double));
    free(old_array);
    return new_array;
}

/**
 * Calculates the arithmetic mean of the data set.
 */
double calculate_mean(const double* data, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

/**
 * Calculates the standard deviation of the data set using the population formula.
 */
double calculate_stddev(const double* data, int size, double mean) {
    double sum_sq = 0;
    for (int i = 0; i < size; i++) {
        double diff = data[i] - mean;
        sum_sq += diff * diff;
    }
    return babylonian_sqrt(sum_sq / size);
}

/**
 * Implements the Babylonian method for calculating square root, for internal use in stddev calculation.
 */
double babylonian_sqrt(double value) {
    double x = value;
    double y = 1.0;
    double e = 0.000001; // Accuracy level
    while (x - y > e) {
        x = (x + y) / 2;
        y = value / x;
    }
    return x;
}

/**
 * Calculates the median of the sorted data set.
 */
double calculate_median(double* data, int size) {
    if (size % 2 == 0) {
        return (data[size / 2 - 1] + data[size / 2]) / 2.0;
    } else {
        return data[size / 2];
    }
}

/**
 * Calculates the mode of the data set. Assumes data is sorted.
 */
double calculate_mode(double* data, int size) {
    double mode = data[0];
    int max_count = 1, count = 1;
    for (int i = 1; i < size; i++) {
        if (data[i] == data[i - 1]) {
            count++;
        } else {
            if (count > max_count) {
                max_count = count;
                mode = data[i - 1];
            }
            count = 1;
        }
    }
    return mode;
}

/**
 * Calculates the harmonic mean of the data set.
 */
double calculate_harmonic_mean(const double* data, int size) {
    double denominator_sum = 0;
    for (int i = 0; i < size; i++) {
        denominator_sum += 1 / data[i];
    }
    return size / denominator_sum;
}

/**
 * Compares two double values for qsort.
 */
int compare_double(const void* a, const void* b) {
    double diff = *(double*)a - *(double*)b;
    return (diff > 0) - (diff < 0);
}

/**
 * Prints the computed results.
 */
void print_results(int size, int capacity, double mean, double median, double mode, double stddev, double harmonic_mean) {
    printf("Results:\n");
    printf("--------\n");
    printf("Num values: %d\n", size);
    printf("Mean: %.3f\n", mean);
    printf("Median: %.3f\n", median);
    printf("Mode: %.3f\n", mode);
    printf("Standard Deviation: %.3f\n", stddev);
    printf("Harmonic Mean: %.3f\n", harmonic_mean);
    printf("Unused array capacity: %d\n", capacity - size);
}
