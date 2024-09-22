#include <CLI/CLI.hpp>
// #include "src/command1.hpp"
// #include "src/command2.hpp"
#include "src/command3.hpp"

int main(int argc, char **argv) {
    CLI::App app{"Babek Application"};

    // // Set up command1
    // CLI::App* cmd1 = app.add_subcommand("command1", "Run command1");
    // Command1* command1 = new Command1(*cmd1);
    // cmd1->callback([command1]() {
    //     command1->execute();
    //     delete command1;
    // });

    // // Set up command2
    // CLI::App* cmd2 = app.add_subcommand("command2", "Run command2");
    // Command2* command2 = new Command2(*cmd2);
    // cmd2->callback([command2]() {
    //     command2->execute();
    //     delete command2;
    // });

    // Set up command2
    CLI::App* cmd3 = app.add_subcommand("command3", "Run command3");
    Command3* command3 = new Command3(*cmd3);
    cmd3->callback([command3]() {
        command3->execute();
        delete command3;
    });

    CLI11_PARSE(app, argc, argv);

    return 0;
}