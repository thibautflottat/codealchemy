#include "parallel.hpp"
#include <iostream>

namespace Parallel {

    void initialize(int& rank, int& size) {
        MPI_Init(nullptr, nullptr);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        std::cout << "MPI initialized: rank " << rank << " of " << size << " ranks." << std::endl;
    }

    void finalize() {
        MPI_Finalize();
        std::cout << "MPI finalized." << std::endl;
    }

    void distribute_frames(int rank, int size, size_t total_frames, size_t& start_frame, size_t& end_frame) {
        size_t frames_per_rank = total_frames / size;
        size_t remainder = total_frames % size;

        start_frame = rank * frames_per_rank + std::min<size_t>(rank, remainder);
        end_frame = start_frame + frames_per_rank + (rank < remainder ? 1 : 0);

        std::cout << "MPI Rank " << rank << " will process frames from " << start_frame << " to " << end_frame - 1 << "." << std::endl;
    }

}