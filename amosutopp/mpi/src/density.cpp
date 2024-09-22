#include "density.hpp"
#include "utils/parallel.hpp"
#include <chemfiles.hpp>
#include <iostream>

Density::Density(CLI::App& app) 
    : Base(), app_ref(app), local_density(0.0), global_density(0.0) {
    // Call parse_common_args from Base using app_ref
    parse_common_args(app_ref);
}

void Density::execute() {
    std::cout << "MPI Rank " << mpi_rank << ": Computing bulk density..." << std::endl;

    init_universe();

    compute_bulk_density();

    MPI_Reduce(&local_density, &global_density, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mpi_rank == 0) {
        std::cout << "Global bulk density: " << global_density << std::endl;
    }
}

void Density::compute_bulk_density() {
    size_t total_frames = trajectory->nsteps();
    size_t start_frame, end_frame;

    Parallel::distribute_frames(mpi_rank, mpi_size, total_frames, start_frame, end_frame);

    for (size_t i = start_frame; i < end_frame; ++i) {
        chemfiles::Frame frame = trajectory->read_step(i);
        std::cout << "MPI Rank " << mpi_rank << " is processing frame " << i << std::endl;

        local_density += (end_frame - start_frame);  // Simplified density calculation
    }
}