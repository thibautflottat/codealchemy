#pragma once

#include <CLI/CLI.hpp>
#include <string>

class Base {
protected:
    int common_arg_1;
    std::string common_arg_2;

public:
    virtual ~Base() = default;

    // Set up common arguments
    virtual void setup(CLI::App *app) {
        app->add_option("-n,--number", common_arg_1, "Common integer argument");
        app->add_option("-s,--string", common_arg_2, "Common string argument");
    }

    // Virtual function to be overridden by derived classes
    virtual void run() = 0;
};