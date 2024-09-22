#include "rdf.hpp"
#include <cmath>
#include <vector>

void compute_rdf_cpu(const std::vector<double>& positions, std::vector<double>& rdf, double bin_size, int num_bins) {
    // Initialize RDF bins to zero
    std::fill(rdf.begin(), rdf.end(), 0.0);

    size_t n_particles = positions.size() / 3;  // assuming 3D coordinates

    // Compute RDF on CPU
    for (size_t i = 0; i < n_particles; ++i) {
        for (size_t j = i + 1; j < n_particles; ++j) {
            double dx = positions[3*i] - positions[3*j];
            double dy = positions[3*i + 1] - positions[3*j + 1];
            double dz = positions[3*i + 2] - positions[3*j + 2];

            double r = std::sqrt(dx*dx + dy*dy + dz*dz);
            int bin = static_cast<int>(r / bin_size);
            if (bin < num_bins) {
                rdf[bin] += 2;  // symmetric count
            }
        }
    }
}