#include "rdf.hpp"
#include <iostream>

void RDF::setup(CLI::App *app) {
    Base::setup(app);
    app->add_flag("--rdf-flag", "Flag specific to RDF");
}

void RDF::run() {
    std::cout << "Running RDF with number: " << common_arg_1 << " and string: " << common_arg_2 << "\n";
}