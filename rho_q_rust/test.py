import numpy as np
import rho_q_rust
import time

Nx = 100000
Nq = 100000

x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros(Nq, dtype=np.complex128)


# Time the execution of the Python code
start_time = time.time()
rho_q_rust.rho_q(x, q, rho)
end_time = time.time()

print(f"Python Execution Time: {end_time - start_time} seconds")