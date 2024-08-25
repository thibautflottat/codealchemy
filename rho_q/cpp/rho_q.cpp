#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cassert>
#include <chrono>
#include <random>
#include <omp.h>

void rho_q(const std::vector<double>& x, 
           const std::vector<double>& q, 
           std::vector<std::complex<double>>& rho, size_t Nx, size_t Nq) {
    #pragma omp parallel for
    for (size_t i = 0; i < Nq; ++i) {
        std::complex<double> rho_value(0.0, 0.0);

        for (size_t j = 0; j < Nx; ++j) {
            double alpha = x[j * 3] * q[i * 3] + x[j * 3 + 1] * q[i * 3 + 1] + x[j * 3 + 2] * q[i * 3 + 2];
            rho_value += std::exp(std::complex<double>(0.0, alpha));
        }

        rho[i] = rho_value;
    }
}

int main() {
    size_t Nx = 10000;
    size_t Nq = 10000;

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
