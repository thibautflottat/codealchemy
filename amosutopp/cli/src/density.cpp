#include "density.hpp"
#include <iostream>

void Density::setup(CLI::App *app) {
    Base::setup(app);
    app->add_flag("--density-flag", "Flag specific to Density");
}

void Density::run() {
    std::cout << "Running Density with number: " << common_arg_1 << " and string: " << common_arg_2 << "\n";
}