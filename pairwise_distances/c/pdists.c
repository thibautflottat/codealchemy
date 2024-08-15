#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>


void pairwise_distances(int n_particles, float* positions, float* distances, float* exec_time) {
    int i, j;
    float L = 1.0;
    float half_L = 0.5;
    float dx, dy, dz, dist;

    // Initialize distances to zero
    for (i = 0; i < n_particles * n_particles; i++) {
        distances[i] = 0.0;
    }

    struct timespec start, end;

    // Start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for private(i, j, dx, dy, dz, dist) shared(n_particles, positions, distances) schedule(dynamic)
    for (i = 0; i < n_particles; i++) {
        for (j = i + 1; j < n_particles; j++) {
            dx = positions[i * 3 + 0] - positions[j * 3 + 0];
            dy = positions[i * 3 + 1] - positions[j * 3 + 1];
            dz = positions[i * 3 + 2] - positions[j * 3 + 2];

            // Apply periodic boundary conditions
            if (dx > half_L) dx -= L;
            if (dx < -half_L) dx += L;
            if (dy > half_L) dy -= L;
            if (dy < -half_L) dy += L;
            if (dz > half_L) dz -= L;
            if (dz < -half_L) dz += L;

            dist = sqrt(dx * dx + dy * dy + dz * dz);
            distances[i * n_particles + j] = dist;
            distances[j * n_particles + i] = dist;
        }
    }

    // End time
    clock_t end_clock = clock();

    // End time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate execution time
    *exec_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9f;
    
    // Debugging: Print the distances matrix
    // for (i = 0; i < n_particles; i++) {
    //     for (j = 0; j < n_particles; j++) {
    //         printf("distances(%d, %d) = %f\n", i, j, distances[i * n_particles + j]);
    //     }
    // }
}
