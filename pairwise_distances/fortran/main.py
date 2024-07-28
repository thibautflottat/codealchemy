import numpy as np
from pairwise_module import pairwise

# Example usage
n_particles = 40000
positions = np.random.rand(n_particles, 3).astype(np.float32)

pairwise.pairwise_distances(positions, n_particles)

# print("Execution time (seconds):", exec_time[0])
