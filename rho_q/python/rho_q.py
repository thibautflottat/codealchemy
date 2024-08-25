import numpy as np
import numba
import time

# Define the dot product function
@numba.njit(fastmath=True, nogil=True)
def dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

# Define the parallelized function to compute rho for all q-points
@numba.njit(fastmath=True, nogil=True)
def rho_q(x: np.ndarray, q: np.ndarray, rho: np.ndarray):
    Nx = len(x)
    Nq = len(q)
    for i in numba.prange(Nq):
        for j in range(Nx):
            alpha = dot(x[j], q[i])
            rho[i, j] = np.exp(1j * alpha)

# Generate random data
Nx = 10000
Nq = 10000
x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros((Nq, Nx), dtype=np.complex128)

# Time the execution of the Python code
start_time = time.time()
rho_q(x, q, rho)
end_time = time.time()

print(f"Python Execution Time: {end_time - start_time} seconds")
