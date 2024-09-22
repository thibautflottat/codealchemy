#include <CLI/CLI.hpp>
#include "../src/command1/command1.hpp"
// #include "../src/command2/command2.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Babek Command Line Application"};

    CLI::App* cmd1 = app.add_subcommand("command1", "Execute command1");
    Command1::setup_command1(*cmd1);

    // CLI::App* cmd2 = app.add_subcommand("command2", "Execute command2");
    // Command2::setup_command2(*cmd2);
    

    CLI11_PARSE(app, argc, argv);

    return 0;
}