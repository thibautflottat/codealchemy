from numba import prange, njit
import numpy as np 


@njit(fastmath=True, nogil=True)
def compute_msd_per_particle(position, lag, n_frames):

    sqdist_sum = 0.0  # Accumulate squared distances
    count = n_frames - lag  # Number of frames contributing to this lag
    for t in range(count):
        dx = position[t + lag, 0] - position[t, 0]
        dy = position[t + lag, 1] - position[t, 1]
        dz = position[t + lag, 2] - position[t, 2]
        sqdist_sum += dx**2 + dy**2 + dz**2  # Sum of squared displacements

    return sqdist_sum / count


@njit(fastmath=True, nogil=True, parallel=True)
def compute_msd_optimized(positions, lag):
    n_frames = positions.shape[0]
    n_particles = positions.shape[1]

    msds_by_particle = np.zeros((n_frames - 1, n_particles))
    
    for i in prange(n_particles):
        compute_msd_per_particle(positions[:, i, :], lag, n_frames)

    return msds_by_particle