import numpy as np
import time 

def compute_pairwise_distances_vectorized(positions, L):   
    # Create an array of shape (N, N, 3) with all pairwise differences
    delta = positions[:, np.newaxis, :] - positions[np.newaxis, :, :]
    print("delta computed")
    # Apply minimum image convention
    delta = delta - L * np.round(delta / L)
    print("pbc")

    # Compute pairwise distances
    distances = np.sqrt(np.sum(delta ** 2, axis=-1))
    
    return distances

# Example usage
N = 40000  # number of particles
L = 1.0  # length of the cubic box
positions = np.random.rand(N, 3).astype(np.float32) * L  # random positions within the box

start_time = time.time()
distances = compute_pairwise_distances_vectorized(positions, L)
end_time = time.time()

execution_time = end_time - start_time
print(f"Execution time (seconds): {execution_time:.4f}")