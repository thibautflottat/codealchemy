import numba as nb
import math
import numpy as np
import time

@nb.njit(nogil=True, fastmath=True)
def pdist(pos_1, pos_2, box_length):
    dx = pos_1[0] - pos_2[0]
    dy = pos_1[1] - pos_2[1]
    dz = pos_1[2] - pos_2[2]

    # Apply minimum image convention
    dx -= box_length * round(dx / box_length)
    dy -= box_length * round(dy / box_length)
    dz -= box_length * round(dz / box_length)

    r_ij = math.sqrt(dx * dx + dy * dy + dz * dz)

    return r_ij


@nb.njit(parallel=True, nogil=True, fastmath=True)
def compute(positions, box_length, distances):
    for i in nb.prange(positions.shape[0]):
        for j in range(positions.shape[0]):
            distances[i,j] = pdist(positions[i], positions[j], box_length)

 
# Example usage
L = 1.0   # Length of the cubic box
N = 2  # Number of particles
# Initialize particle positions randomly within the box
positions = np.random.rand(N, 3).astype(np.float32) * L
distances = np.zeros((N, N), dtype=np.float32)
compute(positions, L, distances)
del positions
del distances


# Example usage
L = 1.0   # Length of the cubic box
N = 40000  # Number of particles

# Initialize particle positions randomly within the box
positions = np.random.rand(N, 3).astype(np.float32) * L
distances = np.zeros((N, N), dtype=np.float32)

start_time = time.time()
compute(positions, L, distances)
end_time = time.time()

execution_time = end_time - start_time
print(f"Execution time (seconds): {execution_time:.4f}")