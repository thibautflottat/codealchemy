#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "base.hpp"
#include <CLI/CLI.hpp>  // Include CLI11

class Density : public Base {
public:
    // Constructor accepts CLI::App& to use in parse_common_args
    Density(CLI::App& app);

    void execute() override;

private:
    CLI::App& app_ref;  // Store reference to CLI::App

    double local_density;      // Local density for this MPI rank
    double global_density;     // Global density (aggregated from all ranks)

    void compute_bulk_density();
};

#endif // DENSITY_HPP