#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "Base.hpp"

class Density : public Base {
public:
    // Constructor and destructor if needed
    Density();
    ~Density();

    // Override the run method to perform the density analysis
    void run() override;
};

#endif // DENSITY_HPP