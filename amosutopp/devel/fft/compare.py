import numpy as np
from numba import njit, prange
import time

# Original version with Numba: parallelized over lag times and particles
@njit(parallel=True, nogil=True, fastmath=True)
def compute_msd(positions, n_frames):
    n_particles = positions.shape[1]
    msds_by_particle = np.zeros((n_frames - 1, n_particles))
    lagtimes = np.arange(1, n_frames)

    for lag in prange(1, n_frames):
        for i in range(n_particles):
            disp = positions[:-lag, i, :] - positions[lag:, i, :]
            sqdist = np.square(disp).sum(axis=-1)
            msds_by_particle[lag - 1, i] = np.mean(sqdist)

    return msds_by_particle

# Optimized version with fewer memory operations and in-place calculation (parallel over particles only)
@njit(parallel=True, nogil=True, fastmath=True)
def compute_msd_optimized(positions, n_frames):
    n_particles = positions.shape[1]
    msds_by_particle = np.zeros((n_frames - 1, n_particles))
    
    for lag in prange(1, n_frames):
        for i in range(n_particles):
            sqdist_sum = 0.0
            count = n_frames - lag
            for t in range(count):
                dx = positions[t + lag, i, 0] - positions[t, i, 0]
                dy = positions[t + lag, i, 1] - positions[t, i, 1]
                dz = positions[t + lag, i, 2] - positions[t, i, 2]
                sqdist_sum += dx**2 + dy**2 + dz**2
            msds_by_particle[lag - 1, i] = sqdist_sum / count

    return msds_by_particle

# Parallelize over particles, nested loop over lags (latest version)
@njit(parallel=True, nogil=True, fastmath=True)
def compute_msd_parallel_particles(positions, n_frames):
    n_particles = positions.shape[1]
    msds_by_particle = np.zeros((n_frames - 1, n_particles))

    for i in prange(n_particles):
        for lag in range(1, n_frames):
            sqdist_sum = 0.0
            count = n_frames - lag
            for t in range(count):
                dx = positions[t + lag, i, 0] - positions[t, i, 0]
                dy = positions[t + lag, i, 1] - positions[t, i, 1]
                dz = positions[t + lag, i, 2] - positions[t, i, 2]
                sqdist_sum += dx**2 + dy**2 + dz**2
            msds_by_particle[lag - 1, i] = sqdist_sum / count

    return msds_by_particle

# Benchmarking section to compare all implementations
def benchmark():
    # Simulate random positions (1000 frames, 100 particles, 3D positions)
    positions = np.random.rand(1000, 10000, 3)
    n_frames = positions.shape[0]

    # Original version
    start = time.time()
    msds_by_particle = compute_msd(positions, n_frames)
    end = time.time()
    print("Original version time:", end - start)

    # Optimized version
    start = time.time()
    msds_by_particle_optimized = compute_msd_optimized(positions, n_frames)
    end = time.time()
    print("Optimized version time (optimized memory):", end - start)

    # Final version with parallelization over particles and nested lags
    start = time.time()
    msds_by_particle_parallel = compute_msd_parallel_particles(positions, n_frames)
    end = time.time()
    print("Parallelized version time (parallel over particles, nested lags):", end - start)



benchmark()