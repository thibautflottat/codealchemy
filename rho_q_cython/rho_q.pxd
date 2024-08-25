# rho_q.pxd
from libcpp.vector cimport vector
from libcpp.complex cimport complex

cdef extern from "rho_q_cpp.hpp":
    void rho_q(const vector[double]& x, 
               const vector[double]& q, 
               vector[complex[double]]& rho, size_t Nx, size_t Nq)