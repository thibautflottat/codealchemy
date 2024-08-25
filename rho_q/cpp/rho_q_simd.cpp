#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cassert>
#include <chrono>
#include <random>
#include <omp.h>
#include <immintrin.h>

inline double dot_avx2(const double* x, const double* q) {
    __m256d vx = _mm256_loadu_pd(x);
    __m256d vq = _mm256_loadu_pd(q);
    __m256d prod = _mm256_mul_pd(vx, vq);

    __m256d sum = _mm256_hadd_pd(prod, prod);
    return _mm256_cvtsd_f64(_mm256_add_pd(sum, _mm256_permute2f128_pd(sum, sum, 0x1)));
}

void rho_q(const std::vector<double>& x, 
           const std::vector<double>& q, 
           std::vector<std::complex<double>>& rho, size_t Nx, size_t Nq) {
    #pragma omp parallel for
    for (size_t i = 0; i < Nq; ++i) {
        std::complex<double> rho_value(0.0, 0.0);

        for (size_t j = 0; j < Nx; j += 4) {
            double alpha1 = dot_avx2(&x[j * 3], &q[i * 3]);
            double alpha2 = dot_avx2(&x[(j + 1) * 3], &q[i * 3]);
            double alpha3 = dot_avx2(&x[(j + 2) * 3], &q[i * 3]);
            double alpha4 = dot_avx2(&x[(j + 3) * 3], &q[i * 3]);

            rho_value += std::exp(std::complex<double>(0.0, alpha1)) +
                         std::exp(std::complex<double>(0.0, alpha2)) +
                         std::exp(std::complex<double>(0.0, alpha3)) +
                         std::exp(std::complex<double>(0.0, alpha4));
        }

        rho[i] = rho_value;
    }
}

int main() {
    size_t Nx = 100000;
    size_t Nq = 100000;

    std::vector<double> x(Nx * 3);
    std::vector<double> q(Nq * 3);
    std::vector<std::complex<double>> rho(Nq, 0.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < Nx * 3; ++i) {
        x[i] = dis(gen);
    }

    for (size_t i = 0; i < Nq * 3; ++i) {
        q[i] = dis(gen);
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    rho_q(x, q, rho, Nx, Nq);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "C++ Execution Time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
