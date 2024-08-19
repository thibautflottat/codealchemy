# rho_q_wrapper.pyx
# distutils: language = c++
# cython: language_level=3

# Import the necessary modules from Cython
from libcpp.vector cimport vector
from libcpp.complex cimport complex

# Include the C++ header to access the function
cdef extern from "rho_q.h":
    void rho_q(const vector[vector[double]]& x, 
               const vector[vector[double]]& q, 
               vector[complex[double]]& rho)

# Define a Python wrapper for the C++ function
def compute_rho_q(list x, list q):
    # Convert Python lists to C++ vectors
    cdef vector[vector[double]] x_cpp
    cdef vector[vector[double]] q_cpp
    cdef vector[complex[double]] rho_cpp
    
    # Convert Python lists to C++ vectors
    x_cpp = vector[vector[double]](len(x))
    for i in range(len(x)):
        x_cpp[i] = vector[double](x[i])
    
    q_cpp = vector[vector[double]](len(q))
    for i in range(len(q)):
        q_cpp[i] = vector[double](q[i])

    # Resize the rho_cpp vector to match the number of q points
    rho_cpp.resize(len(q))
    
    # Call the C++ function
    rho_q(x_cpp, q_cpp, rho_cpp)
    
    # Convert the C++ vector back to a Python list of complex numbers and return
    return [complex(val.real(), val.imag()) for val in rho_cpp]