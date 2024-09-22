#include "rmsd_analysis.hpp"
#include <iostream>
#include </opt/homebrew/Cellar/open-mpi/5.0.3_1/include/mpi.h>

void RMSDAnalysis::analyze_frame(const chemfiles::Frame& frame, size_t frame_number) {
    // Get the MPI rank
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Output message with MPI rank and frame number
    std::cout << "MPI process with rank " << world_rank
              << " is performing RMSD analysis on frame number " << frame_number << "." << std::endl;

    // RMSD analysis logic here
}