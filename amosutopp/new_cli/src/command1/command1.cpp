#include "command1.hpp"
#include <iostream>

Command1::Command1() : Base() {}

void Command1::execute() {
    // Topology and trajectory are already initialized by the base class
    std::cout << "Executing Command1 with topo: " << base_args.topo << " and traj: " << base_args.traj << std::endl;

    // Use topology and trajectory directly
}

void Command1::setup_command1(CLI::App& app) {
    Command1* cmd1 = new Command1();  // No need to pass CLI::App to Base

    // Parse common arguments (inherited from base class)
    cmd1->parse_common_args(app);

    // Call the base method to set up the callback (no redundant logic)
    cmd1->setup_command(app);
}