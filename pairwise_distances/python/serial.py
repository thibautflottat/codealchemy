import numpy as np
import time
import tqdm 

def compute_pairwise_distances(positions, L):
    N = positions.shape[0]
    distances = np.zeros((N, N))
    
    for i in tqdm.tqdm(range(N)):
        for j in range(i + 1, N):
            delta = positions[i] - positions[j]
            # Apply minimum image convention
            delta = delta - L * np.round(delta / L)
            distance = np.linalg.norm(delta)
            distances[i, j] = distance
            distances[j, i] = distance

    return distances

# Example usage
N = 4000  # number of particles
L = 1.0  # length of the cubic box
positions = np.random.rand(N, 3).astype(np.float32) * L  # random positions within the box

start_time = time.time()
distances = compute_pairwise_distances(positions, L)
end_time = time.time()

execution_time = end_time - start_time
print(f"Execution time (seconds): {execution_time:.4f}")