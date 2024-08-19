import numpy as np
cimport numpy as np
cimport cython
from libc.math cimport exp
from cython.cimports.cpython cimport PyComplex_FromDoubles
from cmath import exp as cexp

@cython.boundscheck(False)  # Disable bounds checking for performance
@cython.wraparound(False)   # Disable negative indexing checking for performance
cdef double dot(double[:] a, double[:] b):
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]

@cython.boundscheck(False)
@cython.wraparound(False)
def rho_q(np.ndarray[double, ndim=2] x, np.ndarray[double, ndim=2] q, np.ndarray[complex, ndim=1] rho):
    cdef int Nx = x.shape[0]
    cdef int Nq = q.shape[0]
    cdef int i, j
    cdef double alpha
    cdef complex exp_result
    
    for i in range(Nq):
        for j in range(Nx):
            alpha = dot(x[j, :], q[i, :])
            exp_result = cexp(complex(0.0, alpha))
            rho[i] += exp_result