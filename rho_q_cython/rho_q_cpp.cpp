// rho_q.cpp
#include "rho_q_cpp.hpp"

void rho_q(const std::vector<double>& x, 
           const std::vector<double>& q, 
           std::vector<std::complex<double>>& rho, size_t Nx, size_t Nq) {

    for (size_t i = 0; i < Nq; ++i) {
        std::complex<double> rho_value(0.0, 0.0);

        for (size_t j = 0; j < Nx; ++j) {
            double alpha = x[j * 3] * q[i * 3] + x[j * 3 + 1] * q[i * 3 + 1] + x[j * 3 + 2] * q[i * 3 + 2];
            rho_value += std::exp(std::complex<double>(0.0, alpha));
        }

        rho[i] = rho_value;
    }
}