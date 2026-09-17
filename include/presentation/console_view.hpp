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
    struct id_args;

    void handle_add(const add_args& args) const;
    void handle_list() const;
    void handle_show(const id_args& args) const;
    void handle_delete(const id_args& args) const;

    app::password_service& service_;
};

}  // namespace pm::presentation
