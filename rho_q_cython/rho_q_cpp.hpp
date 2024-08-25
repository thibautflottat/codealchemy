// rho_q.hpp
#ifndef RHO_Q_HPP
#define RHO_Q_HPP

#include <vector>
#include <complex>

void rho_q(const std::vector<double>& x, 
           const std::vector<double>& q, 
           std::vector<std::complex<double> >& rho, // Notice the space here
           size_t Nx, size_t Nq);

#endif