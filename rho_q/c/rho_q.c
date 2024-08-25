#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"
#include <time.h>

void rho_q(const double* x, const double* q, double complex* rho, size_t Nx, size_t Nq) {
    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < Nq; ++i) {
        double complex rho_value = 0.0 + 0.0 * I;

        for (size_t j = 0; j < Nx; ++j) {
            double alpha = x[j * 3] * q[i * 3] + x[j * 3 + 1] * q[i * 3 + 1] + x[j * 3 + 2] * q[i * 3 + 2];
            rho_value += cexp(I * alpha);
        }

        rho[i] = rho_value;
    }
}

int main() {
    size_t Nx = 10000;
    size_t Nq = 10000;

    double* x = (double*)malloc(Nx * 3 * sizeof(double));
    double* q = (double*)malloc(Nq * 3 * sizeof(double));
    double complex* rho = (double complex*)malloc(Nq * sizeof(double complex));

    if (x == NULL || q == NULL || rho == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    srand(time(NULL));

    for (size_t i = 0; i < Nx * 3; ++i) {
        x[i] = (double)rand() / RAND_MAX;
    }

    for (size_t i = 0; i < Nq * 3; ++i) {
        q[i] = (double)rand() / RAND_MAX;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    rho_q(x, q, rho, Nx, Nq);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("C Execution Time: %f seconds\n", elapsed);

    free(x);
    free(q);
    free(rho);

    return 0;
}