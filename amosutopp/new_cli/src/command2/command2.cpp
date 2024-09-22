#include "command2.hpp"
#include <iostream>

static Command2 cmd2;

Command2::Command2() {}

void Command2::execute() {
    std::cout << "Executing Command1 with topo: " << base_args.topo
              << " and traj: " << base_args.traj << std::endl;

    // Initialize the chemfiles objects
    create_universe();

    // Now you can use `topology` and `trajectory` objects in the rest of your code
}

void Command2::setup_command2(CLI::App& app) {
    cmd2.parse_common_args(app);
    app.add_option("--extra1", cmd2.command2_args.extra_arg1, "Extra argument for command1");
    app.callback([]() { cmd2.execute(); });
}