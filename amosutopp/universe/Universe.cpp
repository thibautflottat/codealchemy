#include "universe.hpp"
#include <iostream>

// Constructor: loads the trajectory and topology
Universe::Universe(const std::string& trajectory_file, const std::string& topology_file)
    : trajectory(trajectory_file), current_frame_index(0) {
    // Set the topology
    trajectory.set_topology(topology_file, "LAMMPS Data");

    // Get the number of frames in the trajectory
    n_frames = trajectory.nsteps();

    // Read the topology once (for atom data)
    topology = trajectory.read().topology();

    // Store the masses of atoms
    masses.clear();
    for (size_t i = 0; i < topology.size() / 10000; ++i) {
        masses.push_back(topology[i].mass());
    }
}

// Access the total number of frames
size_t Universe::n_frames_total() const {
    return n_frames;
}

// Get the current frame number
size_t Universe::current_frame_number() const {
    return current_frame_index;
}

// Set the current frame number
void Universe::set_frame_number(size_t frame_index) {
    current_frame_index = frame_index;
}

// Get the current frame
chemfiles::Frame Universe::current_frame() {
    return trajectory.read_step(current_frame_index);
}

// Get the number of atoms in the current frame
size_t Universe::n_atoms() const {
    return topology.size();  // Number of atoms in the topology
}

// Get the atom masses in the current frame
const std::vector<double>& Universe::atom_masses() const {
    return masses;
}

// Print basic info about the universe
void Universe::print_info() const {
    std::cout << "Total frames in trajectory: " << n_frames_total() << std::endl;
    std::cout << "Current frame index: " << current_frame_number() << std::endl;
    std::cout << "Number of atoms in current frame: " << n_atoms() << std::endl;

    std::cout << "Atom masses: ";
    for (const auto& mass : atom_masses()) {
        std::cout << mass << " ";
    }
    std::cout << std::endl;
}