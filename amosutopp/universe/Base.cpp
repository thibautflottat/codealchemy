#include "Base.hpp"

// Destructor to clean up the Universe object
Base::~Base() {
    delete universe;
}

void Base::add_common_args(CLI::App* app) {
    app->add_option("-t,--trajectory", trajectory_file, "Path to the trajectory file")
        ->required();
    app->add_option("-p,--topology", topology_file, "Path to the topology file")
        ->required();
}

void Base::init_universe() {
    universe = new Universe(trajectory_file, topology_file);
}