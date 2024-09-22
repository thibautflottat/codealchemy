#ifndef COMMAND3_HPP
#define COMMAND3_HPP

#include "base.hpp"
#include <CLI/CLI.hpp>

class Command3 : public Base {
public:
    Command3(CLI::App& app);
    void execute() override;

private:
    std::string extra_arg3;  // Extra argument specific to command2
};

#endif // COMMAND3_HPP