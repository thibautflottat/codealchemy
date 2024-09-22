#include "command2.hpp"
#include <iostream>

Command2::Command2(CLI::App& app) : extra_arg1{} {
    parse_common_args(app);  // Parse common arguments
    app.add_option("--extra1", extra_arg1, "Extra argument 1 for Command2")->required();
}

void Command2::execute() {
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
        std::cout << "MPI Rank " << mpi_rank << " processed step " << step
                  << " with extra argument: " << extra_arg1 << std::endl;
    }

    // Finalize MPI
    MPI_Finalize();
}