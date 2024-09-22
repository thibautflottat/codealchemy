
#include <iostream>
#include <chemfiles.hpp>
#include <vector>
#include <boost/multi_array.hpp>
#include <chrono> // For time measurement

int main() {
    // Open the trajectory file
    chemfiles::Trajectory trajectory("/Users/thibaut/opt/dev/codealchemy/amosutopp/devel/cfiles/static.dcd");

    // Get the total number of frames and the number of particles
    size_t n_frames = trajectory.nsteps();
    auto frame = trajectory.read_step(0);
    size_t n_particles = frame.size(); // Number of particles in a frame

    // ---------------------------------------------
    // Implementation 1: Using Flattened std::vector
    // ---------------------------------------------

    auto start_flattened = std::chrono::high_resolution_clock::now(); // Start time

    // Preallocate a 1D vector for positions (flattened 3D array: n_frames * n_particles * 3)
    std::vector<float> positions_flattened(n_frames * n_particles * 3, 0.0f);

    // Loop over all frames to accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        std::cout << "Processing frame " << i << " (Flattened std::vector)" << std::endl;

        // Read the i-th frame
        frame = trajectory.read_step(i);

        // Get the positions of all atoms as a span (view)
        auto current_positions = frame.positions();

        // Precompute the base index for the current frame
        size_t base_index = i * n_particles * 3;

        // Add the current positions to the preallocated positions array
        for (size_t j = 0; j < n_particles; ++j) {
            size_t particle_base_index = base_index + j * 3;
            positions_flattened[particle_base_index + 0] += current_positions[j][0]; // x-coordinate
            positions_flattened[particle_base_index + 1] += current_positions[j][1]; // y-coordinate
            positions_flattened[particle_base_index + 2] += current_positions[j][2]; // z-coordinate
        }
    }

    auto end_flattened = std::chrono::high_resolution_clock::now(); // End time
    std::chrono::duration<double> duration_flattened = end_flattened - start_flattened;

    // ---------------------------------------------
    // Implementation 2: Using boost::multi_array
    // ---------------------------------------------

    auto start_multiarray = std::chrono::high_resolution_clock::now(); // Start time

    // Preallocate a 3D boost::multi_array for positions (n_frames x n_particles x 3)
    boost::multi_array<float, 3> positions_multi(boost::extents[n_frames][n_particles][3]);

    // Loop over all frames to accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        std::cout << "Processing frame " << i << " (boost::multi_array)" << std::endl;

        // Read the i-th frame
        frame = trajectory.read_step(i);

        // Get the positions of all atoms as a span (view)
        auto current_positions = frame.positions();

        // Add the current positions to the preallocated boost::multi_array
        for (size_t j = 0; j < n_particles; ++j) {
            positions_multi[i][j][0] += current_positions[j][0]; // x-coordinate
            positions_multi[i][j][1] += current_positions[j][1]; // y-coordinate
            positions_multi[i][j][2] += current_positions[j][2]; // z-coordinate
        }
    }

    auto end_multiarray = std::chrono::high_resolution_clock::now(); // End time
    std::chrono::duration<double> duration_multiarray = end_multiarray - start_multiarray;
    std::cout << "\nTime taken by Flattened std::vector: " << duration_flattened.count() << " seconds\n";
    std::cout << "\nTime taken by boost::multi_array: " << duration_multiarray.count() << " seconds\n";

    return 0;
}


#include <iostream>
#include <chemfiles.hpp>
#include <vector>
#include <experimental/mdspan>  // Include mdspan (C++23 or external implementation)

// Alias for the type of mdspan we'll use (3D)
using mdspan_3d = std::experimental::mdspan<float, std::experimental::dextents<size_t, 3>>;

int main() {
    // Open the trajectory file
    chemfiles::Trajectory trajectory("/path/to/static.dcd");

    // Get the total number of frames and the number of particles
    size_t n_frames = trajectory.nsteps();
    auto frame = trajectory.read_step(0);
    size_t n_particles = frame.size(); // Number of particles in a frame

    // Preallocate a 1D vector for positions (flattened 3D array: n_frames * n_particles * 3)
    std::vector<float> positions_flattened(n_frames * n_particles * 3, 0.0f);

    // Loop over all frames to accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        frame = trajectory.read_step(i);
        auto current_positions = frame.positions();
        size_t base_index = i * n_particles * 3;
        for (size_t j = 0; j < n_particles; ++j) {
            size_t particle_base_index = base_index + j * 3;
            positions_flattened[particle_base_index + 0] += current_positions[j][0];
            positions_flattened[particle_base_index + 1] += current_positions[j][1];
            positions_flattened[particle_base_index + 2] += current_positions[j][2];
        }
    }

    // Create a 3D mdspan view over the flattened positions array
    mdspan_3d positions_view(positions_flattened.data(), n_frames, n_particles, 3);

    // Example of accessing the accumulated positions using mdspan (print the first frame)
    for (size_t j = 0; j < n_particles; ++j) {
        std::cout << "Particle " << j << ": x = " 
                  << positions_view(0, j, 0) << ", y = "
                  << positions_view(0, j, 1) << ", z = "
                  << positions_view(0, j, 2) << std::endl;
    }

    return 0;
}