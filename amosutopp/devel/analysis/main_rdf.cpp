#include "rdf.hpp"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // Example particle positions (in 3D)
    std::vector<double> positions = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0};  // 3 particles
    double bin_size = 0.1;
    int num_bins = 100;

    std::vector<double> rdf_cpu(num_bins, 0.0);
    std::vector<double> rdf_gpu(num_bins, 0.0);

    // Compute RDF on CPU
    compute_rdf_cpu(positions, rdf_cpu, bin_size, num_bins);

    // Compute RDF on GPU
    compute_rdf_gpu(positions, rdf_gpu, bin_size, num_bins);

    // Output results
    std::cout << "CPU RDF:" << std::endl;
    for (int i = 0; i < num_bins; ++i) {
        std::cout << i * bin_size << " " << rdf_cpu[i] << std::endl;
    }

    std::cout << "GPU RDF:" << std::endl;
    for (int i = 0; i < num_bins; ++i) {
        std::cout << i * bin_size << " " << rdf_gpu[i] << std::endl;
    }

    return 0;
}