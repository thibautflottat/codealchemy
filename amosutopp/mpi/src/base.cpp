#include "base.hpp"
#include <iostream>
#include <stdexcept>

// Base::Base() : base_args{}, trajectory(nullptr), mpi_rank(0), mpi_size(1) {}
Base::Base() : base_args{}, trajectory(nullptr) {}

Base::~Base() {}

void Base::parse_common_args(CLI::App& app) {
    // app.add_option("--topo", base_args.topo, "Topology file")->required()->check(CLI::ExistingFile);
    app.add_option("--traj", base_args.traj, "Trajectory file")->required()->check(CLI::ExistingFile);
}

void Base::init_universe() {
    // if (base_args.traj.empty() || base_args.topo.empty()) {
    //     throw std::runtime_error("Error: Both topology and trajectory files must be provided.");
    // }

    // Initialize Chemfiles trajectory
    trajectory = std::make_unique<chemfiles::Trajectory>(base_args.traj);
    // std::cout << "MPI Rank " << mpi_rank << " initialized the trajectory." << std::endl;
}