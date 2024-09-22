
#include <iostream>
#include <vector>
#include <boost/multi_array.hpp>
#include <chrono>  // For time measurement

// Function to calculate the index in a flattened 3D array
inline size_t index_3d(size_t i, size_t j, size_t k, size_t n_particles, size_t n_dim) {
    return i * n_particles * n_dim + j * n_dim + k;
}

int main() {
    size_t n_frames = 100;
    size_t n_particles = 1000;
    size_t n_dim = 3;

    // Simulated current positions for a frame
    std::vector<float> current_positions(n_particles * n_dim);
    for (size_t j = 0; j < n_particles; ++j) {
        current_positions[j * n_dim + 0] = 1.0f; // x-coordinate
        current_positions[j * n_dim + 1] = 2.0f; // y-coordinate
        current_positions[j * n_dim + 2] = 3.0f; // z-coordinate
    }

    // -------------------------------------------------------------------
    // Approach 1: Using std::vector with manual indexing
    // -------------------------------------------------------------------
    std::vector<float> positions_vector(n_frames * n_particles * n_dim, 0.0f);

    auto start_vector = std::chrono::high_resolution_clock::now();

    // Loop over frames and accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        for (size_t j = 0; j < n_particles; ++j) {
            for (size_t k = 0; k < n_dim; ++k) {
                positions_vector[index_3d(i, j, k, n_particles, n_dim)] += current_positions[j * n_dim + k];
            }
        }
    }

    auto end_vector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_vector = end_vector - start_vector;
    std::cout << "Time taken by std::vector approach: " << duration_vector.count() << " seconds" << std::endl;

    // -------------------------------------------------------------------
    // Approach 2: Using boost::multi_array
    // -------------------------------------------------------------------
    boost::multi_array<float, 3> positions_boost(boost::extents[n_frames][n_particles][n_dim]);

    auto start_boost = std::chrono::high_resolution_clock::now();

    // Loop over frames and accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        for (size_t j = 0; j < n_particles; ++j) {
            for (size_t k = 0; k < n_dim; ++k) {
                positions_boost[i][j][k] += current_positions[j * n_dim + k];
            }
        }
    }

    auto end_boost = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_boost = end_boost - start_boost;
    std::cout << "Time taken by boost::multi_array approach: " << duration_boost.count() << " seconds" << std::endl;

    return 0;
}
