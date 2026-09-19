#include "app/application.hpp"

#include "presentation/repl_session.hpp"

#include <CLI/CLI.hpp>
#include <cstdlib>
#include <iostream>
#include <print>

namespace pm::app {
// clang-format off
int application::run(int argc, char** argv) {
    try {
        if (argc <= 1) {
            run_repl();
            return EXIT_SUCCESS;
        }
        run_single_command(argc, argv);
        return EXIT_SUCCESS;
    } catch (const CLI::ParseError& e) { 
        return e.get_exit_code(); 
    } catch (const std::exception& e) {
        std::print(stderr, "Fatal: {}\n", e.what());
        return EXIT_FAILURE;
    }
}
// clang-format on
void application::run_repl() {
    presentation::repl_session repl{view_};
    repl.run(std::cin, std::cout);
}

void application::run_single_command(int argc, char** argv) {
    CLI::App cli{"Password Manager - secure local password storage"};
    view_.setup_commands(cli);
    cli.parse(argc, argv);
}

}  // namespace pm::app
