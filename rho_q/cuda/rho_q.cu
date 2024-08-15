#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <cuda_runtime.h>
#include <thrust/complex.h>
#include <random>
#include <chrono>

// CUDA kernel to compute rho_q on the GPU
__global__ void rho_q_kernel(const double* x, const double* q, thrust::complex<double>* rho, size_t Nx, size_t Nq) {
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < Nq) {
        thrust::complex<double> rho_value(0.0, 0.0);

        for (size_t j = 0; j < Nx; ++j) {
            double alpha = x[j * 3] * q[i * 3] + x[j * 3 + 1] * q[i * 3 + 1] + x[j * 3 + 2] * q[i * 3 + 2];
            rho_value += thrust::exp(thrust::complex<double>(0.0, alpha));
        }

        rho[i] = rho_value;
    }
}

// Function to set up and launch the CUDA kernel
void rho_q_cuda(const std::vector<double>& x, const std::vector<double>& q, std::vector<std::complex<double>>& rho, size_t Nx, size_t Nq) {
    double *d_x, *d_q;
    thrust::complex<double>* d_rho;

    // Allocate device memory
    cudaMalloc(&d_x, Nx * 3 * sizeof(double));
    cudaMalloc(&d_q, Nq * 3 * sizeof(double));
    cudaMalloc(&d_rho, Nq * sizeof(thrust::complex<double>));

    // Copy data to device
    cudaMemcpy(d_x, x.data(), Nx * 3 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_q, q.data(), Nq * 3 * sizeof(double), cudaMemcpyHostToDevice);

    // Set up the execution configuration
    int blockSize = 256;
    int numBlocks = (Nq + blockSize - 1) / blockSize;

    // Launch the kernel
    rho_q_kernel<<<numBlocks, blockSize>>>(d_x, d_q, d_rho, Nx, Nq);

    // Synchronize to ensure all threads have finished
    cudaDeviceSynchronize();

    // Copy the result back to the host
    cudaMemcpy(rho.data(), d_rho, Nq * sizeof(thrust::complex<double>), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_x);
    cudaFree(d_q);
    cudaFree(d_rho);
}

int main() {
    size_t Nx = 100000;
    size_t Nq = 100000;

    std::vector<double> x(Nx * 3);
    std::vector<double> q(Nq * 3);
    std::vector<std::complex<double>> rho(Nq, 0.0);

    // Generate random data for x and q
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < Nx * 3; ++i) {
        x[i] = dis(gen);
    }

    for (size_t i = 0; i < Nq * 3; ++i) {
        q[i] = dis(gen);
    }

    // Time the CUDA execution
    auto start_time = std::chrono::high_resolution_clock::now();
    rho_q_cuda(x, q, rho, Nx, Nq);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "CUDA Execution Time: " << elapsed.count() << " seconds" << std::endl;

    // Output a sample result
    std::cout << "rho[0]: " << rho[0] << std::endl;

    return 0;
}
