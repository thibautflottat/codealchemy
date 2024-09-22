#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib> // For random number generation

// Function to generate a dummy trajectory
std::vector<float> generate_dummy_trajectory(int num_frames, int num_atoms) {
    std::vector<float> trajectory(num_frames * num_atoms * 3);
    for (int i = 0; i < num_frames * num_atoms * 3; ++i) {
        trajectory[i] = static_cast<float>(rand()) / RAND_MAX; // Random position between 0 and 1
    }
    return trajectory;
}

// Function to perform analysis on a given slice of the trajectory
void analyze_trajectory_slice(const std::vector<float>& trajectory_slice, int num_atoms, int num_frames, int rank) {
    for (int i = 0; i < num_frames; ++i) {
        // Print the first atom's position and the frame number
        int base_index = i * num_atoms * 3;
        std::cout << "MPI Rank: " << rank 
                  << ", Frame: " << i 
                  << ", First Atom Position: ("
                  << trajectory_slice[base_index] << ", "
                  << trajectory_slice[base_index + 1] << ", "
                  << trajectory_slice[base_index + 2] << ")\n";
    }
}

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            std::cerr << "Please run the program with 4 MPI processes.\n";
        }
        MPI_Finalize();
        return -1;
    }

    // Configuration
    int total_frames = 100; // Total number of frames in the trajectory
    int num_atoms = 10;     // Number of atoms per frame

    // Generate a dummy trajectory (on rank 0)
    std::vector<float> trajectory;
    if (rank == 0) {
        trajectory = generate_dummy_trajectory(total_frames, num_atoms);
    }

    // Determine the number of frames per process
    int frames_per_process = total_frames / size;
    std::vector<float> local_trajectory(frames_per_process * num_atoms * 3);

    // Scatter the trajectory slices to each process
    MPI_Scatter(trajectory.data(), frames_per_process * num_atoms * 3, MPI_FLOAT,
                local_trajectory.data(), frames_per_process * num_atoms * 3, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    // Analyze the trajectory slice
    analyze_trajectory_slice(local_trajectory, num_atoms, frames_per_process, rank);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}