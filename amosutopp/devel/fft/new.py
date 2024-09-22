import numpy as np
from numba import njit, prange
import time
from tqdm import tqdm

# Original version with Numba
@njit(parallel=True)
def compute_msd(positions, n_frames):
    n_particles = positions.shape[1]
    msds_by_particle = np.zeros((n_frames - 1, n_particles))
    lagtimes = np.arange(1, n_frames)

    for lag in prange(1, n_frames):
        for i in prange(n_particles):
            disp = positions[:-lag, i, :] - positions[lag:, i, :]
            sqdist = np.square(disp).sum(axis=-1)
            msds_by_particle[lag - 1, i] = np.mean(sqdist)

    return msds_by_particle

# Optimized version with fewer memory operations and in-place calculation
@njit(parallel=True, fastmath=True, nogil=True)
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


@njit(parallel=True, fastmath=True, nogil=True)
def compute_moments_parallel_particles(positions, n_frames):
    n_particles = positions.shape[1]
    second_moments = np.zeros((n_frames - 1, n_particles))
    fourth_moments = np.zeros((n_frames - 1, n_particles))

    # Outer loop parallelized over particles
    for i in prange(n_particles):
        for lag in range(1, n_frames):
            sqdist_sum = 0.0  # Accumulate squared distances (second moment)
            quad_dist_sum = 0.0  # Accumulate fourth power of distances (fourth moment)
            count = n_frames - lag  # Number of frames contributing to this lag
            for t in range(count):
                dx = positions[t + lag, i, 0] - positions[t, i, 0]
                dy = positions[t + lag, i, 1] - positions[t, i, 1]
                dz = positions[t + lag, i, 2] - positions[t, i, 2]
                
                dist_squared = dx**2 + dy**2 + dz**2
                sqdist_sum += dist_squared  # Second moment (MSD)
                quad_dist_sum += dist_squared**2  # Fourth moment
                
            second_moments[lag - 1, i] = sqdist_sum / count
            fourth_moments[lag - 1, i] = quad_dist_sum / count

    return second_moments, fourth_moments


# Benchmarking section
def benchmark():
    # Simulate some random positions (1000 frames, 100 particles, 3D positions)
    positions = np.random.rand(100, 100, 3)
    n_frames = positions.shape[0]
    
    # Run original version
    start = time.time()
    msds_by_particle = compute_msd(positions, n_frames)
    end = time.time()
    print("Original version time:", end - start)

    # Run optimized version
    start = time.time()
    msds_by_particle_optimized = compute_msd_optimized(positions, n_frames)
    end = time.time()
    print("Optimized version time:", end - start)

# Execute the benchmark
if __name__ == "__main__":
    benchmark()