import numpy as np
import numba
import time

# Define the dot product function
@numba.njit(fastmath=True, nogil=True)
def dot(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

# Define the function to compute rho at a single q-point
@numba.njit(fastmath=True, nogil=True)
def rho_q_single(x: np.ndarray, q: np.ndarray) -> complex:
    Nx = len(x)
    rho = 0.0j
    for i in range(Nx):
        alpha = dot(x[i], q)
        rho += np.exp(1j * alpha)
    return rho

# Define the parallelized function to compute rho for all q-points
@numba.njit(fastmath=True, nogil=True, parallel=True)
def rho_q(x: np.ndarray, q: np.ndarray, rho: np.ndarray):
    Nx = len(x)
    Nq = len(q)
    for i in numba.prange(Nq):
        rho[i] = rho_q_single(x, q[i])

# Generate random data
Nx = 100000
Nq = 100000
x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros(Nq, dtype=np.complex128)

# Time the execution of the Python code
start_time = time.time()
rho_q(x, q, rho)
end_time = time.time()

print(f"Python Execution Time: {end_time - start_time} seconds")
