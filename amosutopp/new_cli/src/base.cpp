#include "base.hpp"
#include <chemfiles.hpp>

// Base constructor initializes references with nullptr initially
Base::Base() : base_args{}, topology_ref(*(chemfiles::Trajectory*)nullptr), trajectory_ref(*(chemfiles::Trajectory*)nullptr) {}

void Base::parse_common_args(CLI::App& app) {
    app.add_option("--topo", base_args.topo, "Topology file (e.g., LAMMPS Data)")->check(CLI::ExistingFile);
    app.add_option("--traj", base_args.traj, "Trajectory file")->check(CLI::ExistingFile);
}

void Base::init_universe() {
    if (base_args.topo.empty() || base_args.traj.empty()) {
        throw std::runtime_error("Error: Both topology and trajectory files must be provided.");
    }

    // Create unique_ptrs for topology and trajectory
    std::unique_ptr<chemfiles::Trajectory> topology = std::make_unique<chemfiles::Trajectory>(base_args.topo, 'r', "LAMMPS Data");
    std::unique_ptr<chemfiles::Trajectory> trajectory = std::make_unique<chemfiles::Trajectory>(base_args.traj);

    // Assign references to these objects
    topology_ref = *topology;
    trajectory_ref = *trajectory;

    std::cout << "Topology and trajectory initialized." << std::endl;
}