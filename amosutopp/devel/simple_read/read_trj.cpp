#include <iostream>
#include <chemfiles.hpp>
#include <vector>

int main() {
    // Open the static.dcd file (trajectory)
    chemfiles::Trajectory trajectory("/Users/thibaut/opt/dev/codealchemy/amosutopp/devel/cfiles/static.dcd");
    
    // Get the total number of frames and the number of particles
    size_t n_frames = trajectory.nsteps();
    auto frame = trajectory.read_step(0);
    size_t n_particles = frame.size(); // Number of particles in a frame

    // Preallocate a 1D vector for positions (flattened 3D array: n_frames * n_particles * 3)
    std::vector<float> positions(n_frames * n_particles * 3, 0.0f);

    // Loop over all frames to accumulate positions
    for (size_t i = 0; i < n_frames; ++i) {
        std::cout << "Processing frame " << i << std::endl;

        // Read the i-th frame
        frame = trajectory.read_step(i);

        // Get the positions of all atoms as a span (view)
        auto current_positions = frame.positions();

        // Precompute the base index for the current frame
        size_t base_index = i * n_particles * 3;

        // Add the current positions to the preallocated positions array
        for (size_t j = 0; j < n_particles; ++j) {
            size_t particle_base_index = base_index + j * 3;
            positions[particle_base_index + 0] += current_positions[j][0]; // x-coordinate
            positions[particle_base_index + 1] += current_positions[j][1]; // y-coordinate
            positions[particle_base_index + 2] += current_positions[j][2]; // z-coordinate
        }
    }

    // Example of accessing the accumulated positions (print the first frame)
    for (size_t j = 0; j < n_particles; ++j) {
        std::cout << "Particle " << j << ": x = " 
                  << positions[j * 3 + 0] << ", y = "
                  << positions[j * 3 + 1] << ", z = "
                  << positions[j * 3 + 2] << std::endl;
    }

    return 0;
}