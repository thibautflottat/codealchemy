#ifndef BASE_HPP
#define BASE_HPP

#include "universe.hpp"
#include <string>
#include <CLI/CLI.hpp>

class Base {
public:
    // Common options shared by all analysis types
    std::string trajectory_file;
    std::string topology_file;

    // Universe object shared across all analyses
    Universe* universe;

    // Virtual destructor for proper cleanup
    virtual ~Base();

    // Add common arguments (trajectory, topology) to any subcommand
    virtual void add_common_args(CLI::App* app);

    // Initialize the Universe with trajectory and topology
    virtual void init_universe();

    // Pure virtual method for running the analysis
    virtual void run() = 0;
};

#endif // BASE_HPP