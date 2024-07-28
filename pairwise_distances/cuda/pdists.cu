#include <iostream>
#include <cmath>
#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>

__global__ void pairwise_distances_kernel(int n_particles, float* positions, float* distances) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < n_particles && j < n_particles && i < j) {
        float L = 1.0f;
        float half_L = 0.5f;
        float dx = positions[i * 3] - positions[j * 3];
        float dy = positions[i * 3 + 1] - positions[j * 3 + 1];
        float dz = positions[i * 3 + 2] - positions[j * 3 + 2];

        dx = (dx > half_L) ? dx - L : (dx < -half_L) ? dx + L : dx;
        dy = (dy > half_L) ? dy - L : (dy < -half_L) ? dy + L : dy;
        dz = (dz > half_L) ? dz - L : (dz < -half_L) ? dz + L : dz;

        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        distances[i * n_particles + j] = dist;
        distances[j * n_particles + i] = dist;
    }
}

void pairwise_distances(int n_particles, float* d_positions, float* d_distances) {
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((n_particles + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (n_particles + threadsPerBlock.y - 1) / threadsPerBlock.y);

    pairwise_distances_kernel<<<numBlocks, threadsPerBlock>>>(n_particles, d_positions, d_distances);
    cudaDeviceSynchronize();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_particles>" << std::endl;
        return 1;
    }

    int n_particles = std::atoi(argv[1]);
    if (n_particles <= 0) {
        std::cerr << "Number of particles must be a positive integer." << std::endl;
        return 1;
    }

    size_t positions_size = n_particles * 3 * sizeof(float);
    size_t distances_size = n_particles * n_particles * sizeof(float);

    float* h_positions = (float*)malloc(positions_size);
    float* h_distances = (float*)malloc(distances_size);

    // Initialize positions with random values
    for (int i = 0; i < n_particles * 3; i++) {
        h_positions[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    float* d_positions;
    float* d_distances;

    cudaMalloc(&d_positions, positions_size);
    cudaMalloc(&d_distances, distances_size);

    cudaMemcpy(d_positions, h_positions, positions_size, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    pairwise_distances(n_particles, d_positions, d_distances);
    cudaMemcpy(h_distances, d_distances, distances_size, cudaMemcpyDeviceToHost);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    
    std::cout << "Execution time (milliseconds): " << milliseconds << std::endl;

    // Debugging: Print part of the distances matrix
    // for (int i = 0; i < std::min(n_particles, 10); i++) {
    //     for (int j = 0; j < std::min(n_particles, 10); j++) {
    //         std::cout << "distances(" << i << ", " << j << ") = " << h_distances[i * n_particles + j] << std::endl;
    //     }
    // }

    free(h_positions);
    free(h_distances);
    cudaFree(d_positions);
    cudaFree(d_distances);

    return 0;
}
