// presentation/console_view.hpp
#pragma once

#include "app/password_service.hpp"
#include "domain/password.hpp"

#include <CLI/CLI.hpp>
#include <string>

namespace pm::presentation {

class console_view {
public:
    console_view(pm::app::password_service& service) : service_{service} {}

    ~console_view() = default;
    console_view(const console_view&) = delete;
    console_view& operator=(const console_view&) = delete;
    console_view(console_view&&) = delete;
    console_view& operator=(console_view&&) = delete;

    void setup_commands(CLI::App& app);

private:
    struct add_args {
        std::string title_;
        std::string login_;
        std::string password_;
    };

    struct id_args {
        domain::id_type id_{0};
    };

    void handle_add() const;
    void handle_list() const;
    void handle_show() const;
    void handle_delete() const;

    add_args add_args_;
    id_args show_args_;
    id_args delete_args_;

    pm::app::password_service& service_;
};

}  // namespace pm::presentation
