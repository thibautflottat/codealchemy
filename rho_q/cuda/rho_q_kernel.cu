#include <cuda_runtime.h>
#include <thrust/complex.h>
#include <cmath>

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

extern "C" void rho_q_cuda(const double* x, const double* q, thrust::complex<double>* rho, size_t Nx, size_t Nq) {
    double *d_x, *d_q;
    thrust::complex<double>* d_rho;

    // Allocate device memory
    cudaMalloc(&d_x, Nx * 3 * sizeof(double));
    cudaMalloc(&d_q, Nq * 3 * sizeof(double));
    cudaMalloc(&d_rho, Nq * sizeof(thrust::complex<double>));

    // Copy data to device
    cudaMemcpy(d_x, x, Nx * 3 * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_q, q, Nq * 3 * sizeof(double), cudaMemcpyHostToDevice);

    // Set up the execution configuration
    int blockSize = 256;
    int numBlocks = (Nq + blockSize - 1) / blockSize;

    // Launch the kernel
    rho_q_kernel<<<numBlocks, blockSize>>>(d_x, d_q, d_rho, Nx, Nq);

    // Copy the result back to the host
    cudaMemcpy(rho, d_rho, Nq * sizeof(thrust::complex<double>), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_x);
    cudaFree(d_q);
    cudaFree(d_rho);
}
