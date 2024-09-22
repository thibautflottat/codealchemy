#include <iostream>
#include <chemfiles.hpp>

int main() {

        // Load the topology from structure.data
        chemfiles::Trajectory topology_file("structure.data", 'r', "LAMMPS Data");
        auto frame_topology = topology_file.read();  // Read the first frame to get the topology

        // Open the static.dcd file (trajectory)
        chemfiles::Trajectory trajectory("static.dcd");

        trajectory.set_topology("structure.data", "LAMMPS Data");

        auto topology = trajectory.read().topology();
        auto mass = topology[0].mass();

        // Get the total number of frames
        size_t n_frames = trajectory.nsteps();

        for (size_t i = 0; i < n_frames; ++i) {

            // Read the i-th frame
            auto frame = trajectory.read_step(i);

            // Get the positions of all atoms
            auto positions = frame.positions();

            // Get the first atom's position
            auto first_position = positions[0];

            // Get the first atom's name from the topology
            // auto topol = frame_topology.topology();  // Using topology to get the name
            // auto atom_type = topol[0].type();
            // auto atom_mass = topol[0].mass();

            // Print the atom name and its position (x, y, z)
            std::cout << "Frame " << i << ": "
                      << "Atom mass = " << mass
                      << ", Position = ("
                      << first_position[0] << ", "
                      << first_position[1] << ", "
                      << first_position[2] << ")"
                      << std::endl;
        }

    return 0;
}