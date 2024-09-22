#include "Density.hpp"
#include <iostream>

Density::Density() {
    // Constructor implementation if necessary
}

Density::~Density() {
    // Destructor implementation if necessary
}

void Density::run() {
    // Perform the density analysis by iterating over all frames
    for (size_t i = 0; i < universe->n_frames_total(); ++i) {
        universe->set_frame_number(i);
        std::cout << "\nFrame " << i << ":\n";
        universe->print_info();
    }
}