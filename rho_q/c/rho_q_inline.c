#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <math.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"

#define Nx 10000
#define Nq 10000

// Define the dot product function
static inline double dot(const double *a, const double *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Define the function to compute rho for all q-points
void rho_q(const double (*x)[3], const double (*q)[3], double complex (*rho)[Nx]) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < Nq; i++) {
        for (int j = 0; j < Nx; j++) {
            double alpha = dot(x[j], q[i]);
            // Using separate sin and cos for potential performance improvement
            rho[i][j] = cos(alpha) + I * sin(alpha);
        }
    }
}

int main() {
    // Allocate memory for x, q, and rho
    double (*x)[3] = aligned_alloc(32, Nx * sizeof(*x));
    double (*q)[3] = aligned_alloc(32, Nq * sizeof(*q));
    double complex (*rho)[Nx] = aligned_alloc(32, Nq * sizeof(*rho));

    if (!x || !q || !rho) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Generate random data
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = (double)rand() / RAND_MAX;
        }
    }
    for (int i = 0; i < Nq; i++) {
        for (int j = 0; j < 3; j++) {
            q[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Time the execution of the C code
    double start = omp_get_wtime();
    rho_q(x, q, rho);
    double end = omp_get_wtime();

    printf("C Execution Time: %f seconds\n", end - start);

    // Free allocated memory
    free(x);
    free(q);
    free(rho);

    return 0;
}