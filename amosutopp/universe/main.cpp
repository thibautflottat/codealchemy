#include "Density.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char** argv) {
    CLI::App app{"Molecular Dynamics Trajectory Analysis"};

    // Create the density subcommand
    Density density_analysis;
    CLI::App* density_cmd = app.add_subcommand("density", "Run the density analysis");

    // Add common arguments to the density subcommand
    density_analysis.add_common_args(density_cmd);

    // Parse command-line arguments
    CLI11_PARSE(app, argc, argv);

    // Run the appropriate analysis
    if (*density_cmd) {
        density_analysis.init_universe(); // Initialize Universe
        density_analysis.run();           // Run the density analysis
    } else {
        std::cerr << "Please specify a valid subcommand (e.g., 'density')." << std::endl;
    }

    return 0;
}