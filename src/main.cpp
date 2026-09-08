// main.cpp

#include "app/password_service.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "presentation/console_view.hpp"

#include <exception>
#include <print>
#include <string_view>

std::int32_t main(std::int32_t argc, char** argv) {
    try {
        pm::infrastructure::sqlite_repository repository{"passwords.db"};
        pm::app::password_service service{repository};

        pm::presentation::console_view view{service};
        std::vector<std::string_view> args(argv + 1, argv + argc);
        return view.run(args);
    } catch (const std::exception& e) {
        std::print(stderr, "Fatal error: {}\n", e.what());
        return 1;
    }
}
