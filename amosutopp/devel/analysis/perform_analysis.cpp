#include "perform_analysis.hpp"
#include </opt/homebrew/Cellar/open-mpi/5.0.3_1/include/mpi.h>

void perform_analysis(Analysis* analysis, TrajectoryHandler& trajectory_handler, MPIHandler& mpi_handler) {
    // Broadcast the total number of frames
    size_t n_frames = trajectory_handler.get_n_frames();
    MPI_Bcast(&n_frames, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // Calculate the frames each process should handle
    size_t frames_per_process = n_frames / mpi_handler.get_world_size();
    size_t start_frame = mpi_handler.get_world_rank() * frames_per_process;
    size_t end_frame = (mpi_handler.get_world_rank() == mpi_handler.get_world_size() - 1) ? n_frames : start_frame + frames_per_process;

    // Each process processes its assigned frames
    for (size_t i = start_frame; i < end_frame; ++i) {
        auto frame = trajectory_handler.read_frame(i);
        analysis->analyze_frame(frame, i);  // Pass the frame number (i)
    }
}