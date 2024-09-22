#include "command1.hpp"
#include <iostream>

Command1::Command1(CLI::App& app) {
    parse_common_args(app);  // Parse common arguments
}

void Command1::execute() {
    // Initialize MPI
    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    init_universe();  // Initialize Chemfiles trajectory and topology

    size_t total_steps = trajectory->nsteps();
    size_t start_step = mpi_rank * (total_steps / mpi_size);
    size_t end_step = (mpi_rank + 1) * (total_steps / mpi_size);

    for (size_t step = start_step; step < end_step; ++step) {
        chemfiles::Frame frame = trajectory->read_step(step);
        std::cout << "MPI Rank " << mpi_rank << " processed step " << step << std::endl;
    }

    // Finalize MPI
    MPI_Finalize();
}