#ifndef BASE_HPP
#define BASE_HPP

#include "CLI/CLI.hpp"
#include <chemfiles.hpp>
#include <memory>  // For unique_ptr

struct BaseArgs {
    std::string topo;
    std::string traj;
};

class Base {
public:
    Base();
    virtual void execute() = 0;
    virtual ~Base() = default;

protected:
    BaseArgs base_args;
    // Declare references that will be bound after initialization
    chemfiles::Trajectory& topology_ref;
    chemfiles::Trajectory& trajectory_ref;

    void parse_common_args(CLI::App& app);
    void init_universe();  // Initialize topology and trajectory
};

#endif // BASE_HPP