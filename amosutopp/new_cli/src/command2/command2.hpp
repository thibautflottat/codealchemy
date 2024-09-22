#ifndef COMMAND2_HPP
#define COMMAND2_HPP

#include "base.hpp"
#include <CLI/CLI.hpp>

struct Command2Args {
    std::string extra_arg1;
    std::string extra_arg2;
};

class Command2 : public Base {
public:
    Command2();
    void execute() override;
    static void setup_command2(CLI::App& app);

private:
    Command2Args command2_args;  // Use the new struct for additional arguments
};

#endif // COMMAND2_HPP