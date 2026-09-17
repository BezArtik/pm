// main.cpp

#include "app/password_service.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "presentation/console_view.hpp"
#include "presentation/repl_session.hpp"

#include <CLI/CLI.hpp>
#include <iostream>
#include <print>

std::int32_t main(std::int32_t argc, char** argv) {
    try {
        pm::infrastructure::sqlite_repository repository{"passwords.db"};
        pm::app::password_service service{repository};
        pm::presentation::console_view view{service};

        if (argc == 1) {
            pm::presentation::repl_session repl{view};
            repl.run(std::cin, std::cout);
        } else {
            CLI::App app{"Password Manager - secure local password storage"};
            view.setup_commands(app);

            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError& e) { return app.exit(e); }
        }
    } catch (const std::exception& e) {
        std::print(stderr, "Fatal: {}\n", e.what());
        return 1;
    }
    return 0;
}
