#pragma once
#include "base.h"

class Density : public Base {
public:
    void setup(CLI::App *app);  // Declaration of setup
    void run() override;        // Declaration of run method
};