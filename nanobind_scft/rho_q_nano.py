import numpy as np
import time
import rho_q_module
# Generate random data

# Set the seed for reproducibility
np.random.seed(42)


Nx = 100000
Nq = 100000
x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros(Nq, dtype=np.complex128)
# Time the execution of the Cython-wrapped C++ code
start_time = time.time()
rho_q_module.rho_q(x, q, rho)
end_time = time.time()

print(f"C++ via nanobind Execution Time: {end_time - start_time} seconds")
print(f"rho: {rho}")