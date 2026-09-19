#pragma once

#include "app/password_service.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "presentation/console_view.hpp"

namespace pm::app {

class application {
public:
    application() : repository_{"password.db"}, service_{repository_}, view_{service_} {}

    [[nodiscard]] int run(int argc, char** argv);

private:
    void run_repl();
    void run_single_command(int argc, char** argv);

    infrastructure::sqlite_repository repository_;
    password_service service_;
    presentation::console_view view_;
};

}  // namespace pm::app
