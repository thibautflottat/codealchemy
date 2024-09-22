#include "mpi_handler.hpp"
#include </opt/homebrew/Cellar/open-mpi/5.0.3_1/include/mpi.h>
#include <iostream>

MPIHandler::MPIHandler(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Print the rank of the current process
    std::cout << "MPI process with rank " << world_rank << " is starting." << std::endl;
}

MPIHandler::~MPIHandler() {
    MPI_Finalize();
}

int MPIHandler::get_world_size() const {
    return world_size;
}

int MPIHandler::get_world_rank() const {
    return world_rank;
}