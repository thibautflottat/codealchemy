#include <boost/multi_array.hpp>
#include <iostream>

int main() {
    size_t n_frames = 100;
    size_t n_particles = 1000;
    size_t n_dim = 3;

    // Declare a 3D array using boost::multi_array
    boost::multi_array<float, 3> positions(boost::extents[n_frames][n_particles][n_dim]);

    // Simulated current positions for a frame
    std::vector<float> current_positions(n_particles * n_dim);
    
    // Initialize some current_positions for demonstration
    for (size_t j = 0; j < n_particles; ++j) {
        current_positions[j * n_dim + 0] = 1.0f; // x-coordinate
        current_positions[j * n_dim + 1] = 2.0f; // y-coordinate
        current_positions[j * n_dim + 2] = 3.0f; // z-coordinate
    }

    // Loop over frames and accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        for (size_t j = 0; j < n_particles; ++j) {
            for (size_t k = 0; k < n_dim; ++k) {
                positions[i][j][k] += current_positions[j * n_dim + k];
            }
        }
    }

    return 0;
}