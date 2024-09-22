import numpy as np
from numba import njit, prange

@njit(parallel=True)
def compute_msd_optimized(positions, n_frames):
    n_particles = positions.shape[1]
    msds_by_particle = np.zeros((n_frames - 1, n_particles))
    
    for lag in prange(1, n_frames):
        for i in prange(n_particles):
            sqdist_sum = 0.0  # Accumulate squared distances
            count = n_frames - lag  # Number of frames contributing to this lag
            for t in range(count):
                dx = positions[t + lag, i, 0] - positions[t, i, 0]
                dy = positions[t + lag, i, 1] - positions[t, i, 1]
                dz = positions[t + lag, i, 2] - positions[t, i, 2]
                sqdist_sum += dx**2 + dy**2 + dz**2  # Sum of squared displacements

            msds_by_particle[lag - 1, i] = sqdist_sum / count  # Mean for each lag

    return msds_by_particle


positions = np.random.rand(10, 2, 3)
n_frames = positions.shape[0]
# Assuming you have positions and n_frames defined:
msds_by_particle = compute_msd_optimized(positions, n_frames)