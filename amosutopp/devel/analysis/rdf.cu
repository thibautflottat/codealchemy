#include "rdf.hpp"
#include <cuda_runtime.h>
#include <vector>
#include <cmath>

// GPU kernel to compute RDF
__global__ void compute_rdf_kernel(const double* positions, double* rdf, double bin_size, int num_bins, int n_particles) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n_particles) return;

    for (int j = i + 1; j < n_particles; ++j) {
        double dx = positions[3*i] - positions[3*j];
        double dy = positions[3*i + 1] - positions[3*j + 1];
        double dz = positions[3*i + 2] - positions[3*j + 2];

        double r = sqrt(dx*dx + dy*dy + dz*dz);
        int bin = static_cast<int>(r / bin_size);
        if (bin < num_bins) {
            atomicAdd(&rdf[bin], 2.0);
        }
    }
}

void compute_rdf_gpu(const std::vector<double>& positions, std::vector<double>& rdf, double bin_size, int num_bins) {
    size_t n_particles = positions.size() / 3;
    double* d_positions;
    double* d_rdf;

    // Allocate GPU memory
    cudaMalloc(&d_positions, positions.size() * sizeof(double));
    cudaMalloc(&d_rdf, num_bins * sizeof(double));

    // Copy data to GPU
    cudaMemcpy(d_positions, positions.data(), positions.size() * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemset(d_rdf, 0, num_bins * sizeof(double));

    // Launch the RDF kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (n_particles + threadsPerBlock - 1) / threadsPerBlock;
    compute_rdf_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_positions, d_rdf, bin_size, num_bins, n_particles);

    // Copy results back to host
    cudaMemcpy(rdf.data(), d_rdf, num_bins * sizeof(double), cudaMemcpyDeviceToHost);

    // Free GPU memory
    cudaFree(d_positions);
    cudaFree(d_rdf);
}