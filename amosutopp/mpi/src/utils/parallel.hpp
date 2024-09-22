#ifndef PARALLEL_HPP
#define PARALLEL_HPP

#include <mpi.h>

namespace Parallel {

    // Initialize MPI and get rank and size
    void initialize(int& rank, int& size);

    // Finalize MPI at the end of the program
    void finalize();

    // Distribute trajectory frames among MPI ranks
    void distribute_frames(int rank, int size, size_t total_frames, size_t& start_frame, size_t& end_frame);

}

#endif // PARALLEL_HPP