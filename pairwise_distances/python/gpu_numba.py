import numpy as np
from numba import cuda
import time
import math

@cuda.jit(device=True, fastmath=True)
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

@cuda.jit
def kernel(positions, box_length, distances):
    i, j = cuda.grid(2)
    if i < N and j < N:
        distances[i, j] = pdist(positions[i], positions[j], box_length)
        
# Example usage
L = 1.0   # Length of the cubic box
N = 2  # Number of particles

# Initialize particle positions randomly within the box
positions = np.random.rand(N, 3).astype(np.float32) * L
distances = np.zeros((N, N), dtype=np.float32)

# Move data to the device
d_positions = cuda.to_device(positions)
d_distances = cuda.to_device(distances)

# Define the number of threads in a block
threads_per_block = (16, 16)


# Calculate the number of blocks needed
blocks_per_grid_x = (N + threads_per_block[0] - 1) // threads_per_block[0]
blocks_per_grid_y = (N + threads_per_block[1] - 1) // threads_per_block[1]
# Launch the kernel
kernel[(blocks_per_grid_x, blocks_per_grid_y), threads_per_block](
    d_positions, L, d_distances)
# Synchronize to ensure completion
cuda.synchronize()
# Copy the results back to the host
distances = d_distances.copy_to_host()
del distances 
del positions



# Example usage
L = 1.0   # Length of the cubic box
N = 41000  # Number of particles

# Initialize particle positions randomly within the box
positions = np.random.rand(N, 3).astype(np.float32) * L
distances = np.zeros((N, N), dtype=np.float32)

# Move data to the device
d_positions = cuda.to_device(positions)
d_distances = cuda.to_device(distances)

# Define the number of threads in a block
threads_per_block = (16, 16)

# Calculate the number of blocks needed
blocks_per_grid_x = (N + threads_per_block[0] - 1) // threads_per_block[0]
blocks_per_grid_y = (N + threads_per_block[1] - 1) // threads_per_block[1]

start_time = time.time()
# Launch the kernel
kernel[(blocks_per_grid_x, blocks_per_grid_y), threads_per_block](
    d_positions, L, d_distances)

# Synchronize to ensure completion
cuda.synchronize()

# Copy the results back to the host
distances = d_distances.copy_to_host()

end_time = time.time()

execution_time = end_time - start_time
print(f"Execution time (seconds): {execution_time:.4f}")