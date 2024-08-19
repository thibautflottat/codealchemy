// rho_q.h

#include <complex>
#include <vector>
#include <cmath>

void rho_q(const std::vector<std::vector<double>>& x, 
           const std::vector<std::vector<double>>& q, 
           std::vector<std::complex<double>>& rho);