#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <chemfiles.hpp>
#include <vector>

class Universe {
public:
    // Constructor: initializes trajectory and topology
    Universe(const std::string& trajectory_file, const std::string& topology_file);

    // Access the total number of frames
    size_t n_frames_total() const;

    // Get the current frame number
    size_t current_frame_number() const;

    // Set the current frame number
    void set_frame_number(size_t frame_index);

    // Get the current frame
    chemfiles::Frame current_frame();

    // Get the number of atoms in the current frame
    size_t n_atoms() const;

    // Get the atom masses in the current frame
    const std::vector<double>& atom_masses() const;

    // Print basic info about the universe
    void print_info() const;

private:
    chemfiles::Trajectory trajectory;   // Trajectory file handler
    chemfiles::Topology topology;       // Topology data
    size_t n_frames;                    // Number of frames in the trajectory
    size_t current_frame_index;         // Current frame index

    // Atom information
    std::vector<double> masses;         // Masses of atoms in the current frame
};

#endif // UNIVERSE_HPP