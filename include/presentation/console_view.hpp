// presentation/console_view.hpp
#pragma once

#include "app/password_service.hpp"

#include <CLI/CLI.hpp>

namespace pm::presentation {

class console_view {
public:
    console_view(app::password_service& service) : service_{service} {}

    ~console_view() = default;
    console_view(const console_view&) = delete;
    console_view& operator=(const console_view&) = delete;
    console_view(console_view&&) = delete;
    console_view& operator=(console_view&&) = delete;

    void setup_commands(CLI::App& app);
    bool parse_line(const std::string& line);

private:
    struct add_args;
    struct show_args;
    struct delete_args;
    struct list_args;

    void handle(const add_args& args) const;
    void handle(const list_args&) const;
    void handle(const show_args& args) const;
    void handle(const delete_args& args) const;

    app::password_service& service_;
};

}  // namespace pm::presentation
