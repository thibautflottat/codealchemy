#ifndef COMMAND1_HPP
#define COMMAND1_HPP

#include "base.hpp"
#include <CLI/CLI.hpp>

class Command1 : public Base {
public:
    Command1(CLI::App& app);
    void execute() override;
};

#endif // COMMAND1_HPP