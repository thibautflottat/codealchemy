#include <iostream>
#include <immintrin.h>
#include <vector>
#include <cmath>
#include <random>
#include <omp.h>

// Function to compute MSD using AVX and OpenMP for multiple time lags
std::vector<std::vector<float>> compute_MSD_AVX_OpenMP(const std::vector<std::vector<std::vector<float>>>& positions) {
    size_t n_frames = positions.size();
    size_t n_particles = positions[0].size();
    size_t dim = 3; // x, y, z

    // MSD array to store results for each time lag and particle
    std::vector<std::vector<float>> msd(n_frames, std::vector<float>(n_particles, 0.0f));

    // Using OpenMP to parallelize across threads
    #pragma omp parallel
    {
        // Get the thread id and total number of threads
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Split the work among threads
        size_t chunk_size = (n_particles + num_threads - 1) / num_threads;
        size_t start = thread_id * chunk_size;
        size_t end = std::min(start + chunk_size, n_particles);

        // Loop over particles assigned to this thread
        for (size_t p = start; p < end; ++p) {
            // Loop over time lags
            for (size_t tau = 1; tau < n_frames; ++tau) {
                float total_msd = 0.0f;

                // Using AVX (256-bit) - it processes 8 floats at a time
                size_t simd_width = 8;
                size_t simd_iters = ((n_frames - tau) / simd_width) * simd_width;

                for (size_t t = 0; t < simd_iters; t += simd_width) {
                    // Load 8 frames of x, y, z components and compute squared displacement for each particle

                    // For x-component
                    __m256 initial_x = _mm256_loadu_ps(&positions[t][p][0]);    // Initial positions at frame t
                    __m256 current_x = _mm256_loadu_ps(&positions[t + tau][p][0]); // Positions at frame t + tau
                    __m256 diff_x = _mm256_sub_ps(current_x, initial_x);
                    __m256 squared_diff_x = _mm256_mul_ps(diff_x, diff_x);

                    // For y-component
                    __m256 initial_y = _mm256_loadu_ps(&positions[t][p][1]);
                    __m256 current_y = _mm256_loadu_ps(&positions[t + tau][p][1]);
                    __m256 diff_y = _mm256_sub_ps(current_y, initial_y);
                    __m256 squared_diff_y = _mm256_mul_ps(diff_y, diff_y);

                    // For z-component
                    __m256 initial_z = _mm256_loadu_ps(&positions[t][p][2]);
                    __m256 current_z = _mm256_loadu_ps(&positions[t + tau][p][2]);
                    __m256 diff_z = _mm256_sub_ps(current_z, initial_z);
                    __m256 squared_diff_z = _mm256_mul_ps(diff_z, diff_z);

                    // Sum x, y, z squared differences
                    __m256 squared_sum = _mm256_add_ps(squared_diff_x, _mm256_add_ps(squared_diff_y, squared_diff_z));

                    // Horizontal sum of the elements in squared_sum and add it to total MSD
                    float temp[8];
                    _mm256_storeu_ps(temp, squared_sum);
                    for (int i = 0; i < 8; ++i) {
                        total_msd += temp[i];
                    }
                }

                // Handle remaining frames (if not a multiple of 8)
                for (size_t t = simd_iters; t < n_frames - tau; ++t) {
                    float dx = positions[t + tau][p][0] - positions[t][p][0];
                    float dy = positions[t + tau][p][1] - positions[t][p][1];
                    float dz = positions[t + tau][p][2] - positions[t][p][2];

                    total_msd += dx * dx + dy * dy + dz * dz;
                }

                // Store the average result for particle p at lag tau
                msd[tau][p] = total_msd / (n_frames - tau); // Average over all available time frames for this lag
            }
        }
    }

    return msd;
}

// Function to initialize positions using a random walk
std::vector<std::vector<std::vector<float>>> initialize_random_walk(size_t n_frames, size_t n_particles, float step_size = 1.0f) {
    std::vector<std::vector<std::vector<float>>> positions(n_frames, std::vector<std::vector<float>>(n_particles, std::vector<float>(3, 0.0f)));
    
    // Random number generator for the random walk
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-step_size, step_size);

    // Initialize each particle's position at frame 0
    for (size_t p = 0; p < n_particles; ++p) {
        for (size_t d = 0; d < 3; ++d) {
            positions[0][p][d] = 0.0f; // Start at origin
        }
    }

    // Perform random walk for each particle over all frames
    for (size_t f = 1; f < n_frames; ++f) {
        for (size_t p = 0; p < n_particles; ++p) {
            for (size_t d = 0; d < 3; ++d) {
                // Each step is a random displacement in [-step_size, step_size]
                positions[f][p][d] = positions[f-1][p][d] + dis(gen);
            }
        }
    }

    return positions;
}

int main() {
    size_t n_frames = 100;      // Number of frames (time steps)
    size_t n_particles = 1000;  // Number of particles
    float step_size = 1.0f;     // Step size for the random walk

    // Initialize positions using random walk
    std::vector<std::vector<std::vector<float>>> positions = initialize_random_walk(n_frames, n_particles, step_size);

    // Set the number of threads for OpenMP
    omp_set_num_threads(4);

    // Compute MSD using AVX and OpenMP for multiple time lags
    std::vector<std::vector<float>> msd = compute_MSD_AVX_OpenMP(positions);

    // Output results for a few lags
    std::cout << "Mean Squared Displacement (MSD) for each particle at different time lags:" << std::endl;
    for (size_t tau = 1; tau < 10; ++tau) {
        std::cout << "Lag " << tau << ": ";
        for (size_t p = 0; p < 5; ++p) { // Display first 5 particles for simplicity
            std::cout << msd[tau][p] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}