#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <math.h>
#include <omp.h>
#include <immintrin.h>

#define Nx 10000
#define Nq 10000

// SIMD optimized dot product
static inline __m256d dot_avx(const double *a, const double *b) {
    __m256d va = _mm256_loadu_pd(a);
    __m256d vb = _mm256_loadu_pd(b);
    __m256d mult = _mm256_mul_pd(va, vb);
    __m256d sum1 = _mm256_hadd_pd(mult, mult);
    __m256d sum2 = _mm256_permute4x64_pd(sum1, 0b01001110);
    return _mm256_add_pd(sum1, sum2);
}

// Define the function to compute rho for all q-points
void rho_q(const double (*x)[3], const double (*q)[3], double complex (*rho)[Nx]) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < Nq; i++) {
        for (int j = 0; j < Nx; j += 4) {
            __m256d alpha = dot_avx(q[i], x[j]);
            
            double alphas[4];
            _mm256_storeu_pd(alphas, alpha);

            for (int k = 0; k < 4; k++) {
                rho[i][j+k] = cos(alphas[k]) + I * sin(alphas[k]);
            }
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