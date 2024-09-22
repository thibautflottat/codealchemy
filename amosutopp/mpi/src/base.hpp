#ifndef BASE_HPP
#define BASE_HPP

#include <string>
#include <memory>
#include <chemfiles.hpp>  // Chemfiles for reading trajectory and topology
#include <CLI/CLI.hpp>    // CLI11 for argument parsing
// #include <mpi.h>          // MPI for parallel execution

class Base {
public:
    Base();
    virtual ~Base();

    virtual void execute() = 0;  // Pure virtual function for execution

    void parse_common_args(CLI::App& app);  // Argument parsing
    void init_universe();                   // Initialize trajectory and topology
    // void finalize_mpi();                    // Finalize MPI

protected:
    struct BaseArgs {
        std::string topo;
        std::string traj;
    };

    BaseArgs base_args;

    std::unique_ptr<chemfiles::Trajectory> trajectory;  // Chemfiles trajectory
    // int mpi_rank;  // MPI rank
    // int mpi_size;  // MPI size
};

#endif // BASE_HPP