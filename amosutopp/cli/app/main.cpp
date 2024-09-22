#include <CLI/CLI.hpp>
#include "../src/rdf.hpp"       // Include header for RDF
#include "../src/density.hpp"   // Include header for Density

int main(int argc, char **argv) {
    CLI::App app{"CLI for RDF and Density"};

    // RDF subcommand
    RDF rdf;
    auto rdf_cmd = app.add_subcommand("rdf", "Run the RDF module");
    rdf.setup(rdf_cmd); // Setup arguments for RDF
    rdf_cmd->callback([&rdf]() { rdf.run(); }); // Assign callback to run RDF

    // Density subcommand
    Density density;
    auto density_cmd = app.add_subcommand("density", "Run the Density module");
    density.setup(density_cmd); // Setup arguments for Density
    density_cmd->callback([&density]() { density.run(); }); // Assign callback to run Density

    CLI11_PARSE(app, argc, argv);
    return 0;
}