#include <iostream>
#include <chemfiles.hpp>
#include </opt/homebrew/Cellar/open-mpi/5.0.3_1/include/mpi.h>

int main(int argc, char** argv) {

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Open the static.dcd file (trajectory)
    chemfiles::Trajectory trajectory("static.dcd");
    trajectory.set_topology("structure.data", "LAMMPS Data");
    auto topology = trajectory.read().topology();
    auto atom_mass = topology[0].mass();

    // Get the total number of frames (do it only on one process and broadcast it)
    size_t n_frames = trajectory.nsteps();
    MPI_Bcast(&n_frames, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    // Calculate the frames each process should handle
    size_t frames_per_process = n_frames / world_size;
    size_t start_frame = world_rank * frames_per_process;
    size_t end_frame = (world_rank == world_size - 1) ? n_frames : start_frame + frames_per_process;

    // Each process processes its assigned frames
    for (size_t i = start_frame; i < end_frame; ++i) {

        // Read the i-th frame
        auto frame = trajectory.read_step(i);

        // Perform analysis

    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}