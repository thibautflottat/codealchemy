import numpy as np
import time


def dot(a, b):
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]


def rho_q(x, q, rho):
    Nx = len(x)
    Nq = len(q)

    for i in range(Nq):
        for j in range(Nx):
            alpha = dot(x[j], q[i])
            rho[i] += np.exp(1j * alpha)


# Generate random data
Nx = 1000
Nq = 1000
x = np.random.rand(Nx, 3)
q = np.random.rand(Nq, 3)
rho = np.zeros(Nq, dtype=np.complex128)

# Time the execution of the Python code
start_time = time.time()
rho_q(x, q, rho)
end_time = time.time()

print(f"Python Execution Time: {end_time - start_time} seconds")