# rho_q.pyx
from libcpp.vector cimport vector
from libcpp.complex cimport complex as cpp_complex
import numpy as np
cimport numpy as np

cdef extern from "rho_q_cpp.hpp":
    void rho_q(const vector[double]& x, 
               const vector[double]& q, 
               vector[cpp_complex[double]]& rho, size_t Nx, size_t Nq)

def py_rho_q(np.ndarray[np.double_t, ndim=2] x, 
             np.ndarray[np.double_t, ndim=2] q):
    cdef size_t Nx = x.shape[0]
    cdef size_t Nq = q.shape[0]
    
    # Convert the numpy arrays to C++ std::vectors
    cdef vector[double] vec_x
    cdef vector[double] vec_q
    cdef vector[cpp_complex[double]] vec_rho
    
    vec_x.reserve(Nx * 3)
    vec_q.reserve(Nq * 3)
    vec_rho.resize(Nq)
    
    for i in range(Nx):
        vec_x.push_back(x[i, 0])
        vec_x.push_back(x[i, 1])
        vec_x.push_back(x[i, 2])
    
    for i in range(Nq):
        vec_q.push_back(q[i, 0])
        vec_q.push_back(q[i, 1])
        vec_q.push_back(q[i, 2])
    
    # Call the C++ function
    rho_q(vec_x, vec_q, vec_rho, Nx, Nq)
    
    # Convert the C++ result back to a numpy array
    cdef np.ndarray[np.complex128_t, ndim=1] rho = np.empty(Nq, dtype=np.complex128)
    
    for i in range(Nq):
        rho[i] = rho[i] = complex(vec_rho[i].real(), vec_rho[i].imag())
    
    return rho