#ifndef COMMAND1_HPP
#define COMMAND1_HPP

#include "base.hpp"

// Command1 class, derived from Base
class Command1 : public Base {
public:
    Command1();
    void execute() override;  // Specific command logic
    static void setup_command1(CLI::App& app);
};

#endif // COMMAND1_HPP