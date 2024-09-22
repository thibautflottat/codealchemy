import numpy as np
from numba import njit, prange
import time
from tqdm import tqdm


@njit(fastmath=True, nogil=True)
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



@njit(fastmath=True, nogil=True)
def get_msd_traj_nb1(pos):
    result = np.zeros_like(pos)
    deltastop = pos.shape[1]
    for traj in range(pos.shape[0]):
        for dim in range(pos.shape[2]):
            for delta in range(1,deltastop):
                thisresult = 0
                for i in range(delta,deltastop):
                    thisresult += (pos[traj,i,dim] - pos[traj,i-delta,dim])**2
                result[traj,delta,dim] = thisresult / (deltastop - delta)
    return result

# Simulate some random positions (1000 frames, 100 particles, 3D positions)
positions = np.random.rand(1000, 100, 3)
positions_ = np.random.rand(1000, 3, 100)
n_frames = positions.shape[0]

# Run optimized version
start = time.time()
msds_by_particle_optimized = compute_msd_optimized(positions, n_frames)
end = time.time()
print("Optimized version time:", end - start)

start = time.time()
result = get_msd_traj_nb1(positions_)
end = time.time()
print("Optimized version time:", end - start)

