#include <iostream>
#include "mpi_handler.hpp"
#include "trajectory_handler.hpp"
#include "analysis/perform_analysis.hpp"
#include "analysis/rdf_analysis.hpp"
#include "analysis/msd_analysis.hpp"
#include "analysis/rmsd_analysis.hpp"

int main(int argc, char** argv) {
    MPIHandler mpi_handler(argc, argv);
    TrajectoryHandler trajectory_handler("static.dcd", "structure.data");

    // Check the command-line argument for the type of analysis
    if (argc < 2) {
        std::cerr << "Please specify the type of analysis (rdf, msd, rmsd)" << std::endl;
        return 1;
    }

    std::string analysis_type = argv[1];

    // Choose the analysis type dynamically
    Analysis* analysis = nullptr;
    if (analysis_type == "rdf") {
        analysis = new RDFAnalysis();
    } else if (analysis_type == "msd") {
        analysis = new MSDAnalysis();
    } else if (analysis_type == "rmsd") {
        analysis = new RMSDAnalysis();
    } else {
        std::cerr << "Unknown analysis type!" << std::endl;
        return 1;
    }

    // Perform the analysis
    perform_analysis(analysis, trajectory_handler, mpi_handler);

    delete analysis;  // Clean up the allocated analysis object
    return 0;
}