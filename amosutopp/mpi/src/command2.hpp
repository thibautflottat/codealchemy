#ifndef COMMAND2_HPP
#define COMMAND2_HPP

#include "base.hpp"
#include <CLI/CLI.hpp>

class Command2 : public Base {
public:
    Command2(CLI::App& app);
    void execute() override;

private:
    std::string extra_arg1;  // Extra argument specific to command2
};

#endif // COMMAND2_HPP