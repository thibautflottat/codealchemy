#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void pairwise_distances(int n_particles, float* positions, float* distances, float* exec_time);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_particles>\n", argv[0]);
        return 1;
    }

    int n_particles = atoi(argv[1]);
    if (n_particles <= 0) {
        printf("Number of particles must be a positive integer.\n");
        return 1;
    }

    float* positions = (float*)malloc(n_particles * 3 * sizeof(float));
    float* distances = (float*)malloc(n_particles * n_particles * sizeof(float));
    float exec_time;

    // Initialize positions (example: random initialization)
    srand(time(NULL));
    for (int i = 0; i < n_particles * 3; i++) {
        positions[i] = (double)rand() / RAND_MAX;
    }

    // Debugging: Print positions array
    // printf("Positions array:\n");
    // for (int i = 0; i < n_particles; i++) {
    //     printf("positions(%d, :) = [%f, %f, %f]\n", i, positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
    // }

    // Call the pairwise_distances function
    pairwise_distances(n_particles, positions, distances, &exec_time);

    // Print the execution time
    printf("Execution time (seconds): %f\n", exec_time);

    // Free allocated memory
    free(positions);
    free(distances);

    return 0;
}
