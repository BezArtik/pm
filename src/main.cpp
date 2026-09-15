// main.cpp

#include "app/password_service.hpp"
#include "core/errors/error.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "presentation/console_view.hpp"

#include <CLI/CLI.hpp>
#include <print>

std::int32_t main(std::int32_t argc, char** argv) {
    CLI::App app{"Password Manager - secure local password storage"};

    try {
        pm::infrastructure::sqlite_repository repository{"passwords.db"};
        pm::app::password_service service{repository};
        pm::presentation::console_view view{service};

        view.setup_commands(app);
        app.parse(argc, argv);
        // clang-format off
    } catch (const CLI::ParseError& e) { 
        return app.exit(e); 
    } catch (const pm::core::pm_error& e) {
        std::print(stderr, "Error: {}\n", e.what());
        return 1;
    } catch (const std::exception& e) {
        std::print(stderr, "Fatal error: {}\n", e.what());
        return 1;
        // clang-format on
    }
    return 0;
}
