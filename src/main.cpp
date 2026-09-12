// main.cpp

#include "app/password_service.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "presentation/console_view.hpp"

#include <CLI/CLI.hpp>

std::int32_t main(std::int32_t argc, char** argv) {
    CLI::App app{"Password Manager - secure local password storage"};

    try {
        pm::infrastructure::sqlite_repository repository{"passwords.db"};
        pm::app::password_service service{repository};
        pm::presentation::console_view view{service};

        view.setup_commands(app);

        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) { return app.exit(e); }

    return 0;
}
