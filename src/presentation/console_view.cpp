// presentation/console_view.cpp

#include "presentation/console_view.hpp"

#include "CLI/CLI.hpp"

#include <chrono>
#include <format>
#include <print>
#include <ranges>

namespace pm::presentation {

namespace {

auto truncate(std::string_view str, std::size_t max_width) {
    if (str.length() <= max_width) { return std::string{str}; }
    return std::format("{}...", str.substr(0, max_width - 3));
}

auto format_date(pm::domain::time_type timestamp) {
    auto time = std::chrono::system_clock::from_time_t(timestamp);
    return std::format("{:%Y-%m-%d}", time);
}

auto format_add_success(const pm::domain::password_entry& entry) {
    return std::format(
        "Entry added successfully:\n"
        "  ID: {}\n"
        "  Title: {}\n"
        "  Login: {}\n"
        "  Created: {}\n",
        entry.id_, entry.title_, entry.login_.empty() ? "(empty)" : entry.login_, format_date(entry.created_at_));
}

auto format_list(std::ranges::input_range auto&& entries) {
    if (entries.empty()) { return std::string{"No entries found.\n"}; }

    constexpr std::size_t id_width = 4;
    constexpr std::size_t title_width = 30;
    constexpr std::size_t login_width = 20;
    constexpr std::size_t date_width = 10;

    std::string result;
    result.reserve(entries.size() * 4);
    result += std::format("{:<{}} {:<{}} {:<{}} {:<{}}\n", "ID", id_width, "Title", title_width, "Login", login_width,
                          "Created", date_width);
    result += std::format("{:-<{}} {:-<{}} {:-<{}} {:-<{}}\n", "", id_width, "", title_width, "", login_width, "",
                          date_width);

    for (const auto& entry : entries) {
        result += std::format("{:<{}} {:<{}} {:<{}} {:<{}}\n", entry.id_, id_width, truncate(entry.title_, title_width),
                              title_width, truncate(entry.login_.empty() ? "(empty)" : entry.login_, login_width),
                              login_width, format_date(entry.created_at_), date_width);
    }

    return result;
}

auto format_show(const pm::domain::password_entry& entry) {
    return std::format(
        "Entry details:\n"
        "  ID: {}\n"
        "  Title: {}\n"
        "  Login: {}\n"
        "  Password: {}\n"
        "  Created: {}\n",
        entry.id_, entry.title_, entry.login_.empty() ? "(empty)" : entry.login_,
        std::string(entry.password_.length(), '*'), format_date(entry.created_at_));
}

auto format_delete_success(const pm::domain::password_entry& entry) {
    return std::format("Entry deleted successfully: {}\n", entry.title_);
}

[[noreturn]] void pm_error(std::string_view msg) {
    std::print(stderr, "Error: {}\n", msg);
    throw CLI::RuntimeError{};
}

}  // namespace

void console_view::setup_commands(CLI::App& app) {
    // add
    auto* add_cmd = app.add_subcommand("add", "Add a new password entry");
    add_cmd->add_option("--title", add_args_.title_, "Entry title (required)")->required();
    add_cmd->add_option("--login", add_args_.login_, "Login/username");
    add_cmd->add_option("--password", add_args_.password_, "Password (required)")->required();
    add_cmd->callback([this]() { handle_add(); });

    // list
    auto* list_cmd = app.add_subcommand("list", "List all password entries");
    list_cmd->callback([this]() { handle_list(); });

    // show
    auto* show_cmd = app.add_subcommand("show", "Show password entry by ID");
    show_cmd->add_option("--id", show_args_.id_, "Entry ID")->required();
    show_cmd->callback([this]() { handle_show(); });

    // delete
    auto* delete_cmd = app.add_subcommand("delete", "Delete password entry by ID");
    delete_cmd->add_option("--id", delete_args_.id_, "Entry ID")->required();
    delete_cmd->callback([this]() { handle_delete(); });

    app.require_subcommand(1);
}

void console_view::handle_add() const {
    auto result = service_.add_password(add_args_.title_, add_args_.login_, add_args_.password_);
    if (!result) { pm_error(result.error()); }

    std::print("{}", format_add_success(*result));
}

void console_view::handle_list() const {
    std::print("{}", format_list(service_.list_passwords()));
}

void console_view::handle_show() const {
    auto result = service_.get_password(show_args_.id_);
    if (!result) { pm_error(result.error()); }

    std::print("{}", format_show(*result));
}

void console_view::handle_delete() const {
    auto entry = service_.get_password(delete_args_.id_);
    if (!entry) { pm_error(entry.error()); }

    auto result = service_.delete_password(delete_args_.id_);
    if (!result) { pm_error(result.error()); }

    std::print("{}", format_delete_success(*entry));
}

}  // namespace pm::presentation
