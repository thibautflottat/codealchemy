import numpy as np
import time
import rho_q  # Import the compiled Cython module

# Generate random data
Nx = 1000
Nq = 1000
x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros(Nq, dtype=np.complex128)

# Time the execution of the Cython code
start_time = time.time()
rho_q.rho_q(x, q, rho)
end_time = time.time()

print(f"Cython Execution Time: {end_time - start_time} seconds")