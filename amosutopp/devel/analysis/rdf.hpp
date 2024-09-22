#ifndef RDF_HPP
#define RDF_HPP

#include <vector>

void compute_rdf_cpu(const std::vector<double>& positions, std::vector<double>& rdf, double bin_size, int num_bins);
void compute_rdf_gpu(const std::vector<double>& positions, std::vector<double>& rdf, double bin_size, int num_bins);

#endif // RDF_HPP