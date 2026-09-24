module;

#include <chrono>
#include <format>
#include <memory>
#include <print>
#include <string>

module pm.presentation.console_view;

import cli11;
import pm.core.errors;

namespace pm::presentation {

struct console_view::add_args {
    std::string title_;
    std::string login_;
    std::string password_;
};

struct console_view::show_args {
    domain::id_type id_{0};
    bool reveal_{false};
};

struct console_view::delete_args {
    domain::id_type id_{0};
};

struct console_view::list_args {};

void console_view::setup_commands(CLI::App& app) {
    auto&& add_args = std::make_shared<console_view::add_args>();
    auto&& add_cmd = app.add_subcommand("add", "Add a new password entry");
    add_cmd->add_option("--title", add_args->title_, "Entry title (required)")->required();
    add_cmd->add_option("--login", add_args->login_, "Login/username");
    add_cmd->add_option("--password", add_args->password_, "Password (required)")->required();
    add_cmd->callback([this, add_args]() { handle(*add_args); });

    auto&& list_cmd = app.add_subcommand("list", "List all password entries");
    list_cmd->callback([this]() { handle(list_args{}); });

    auto&& show_args = std::make_shared<console_view::show_args>();
    auto&& show_cmd = app.add_subcommand("show", "Show password entry by ID");
    show_cmd->add_option("--id", show_args->id_, "Entry ID")->required();
    show_cmd->add_flag("--reveal", show_args->reveal_, "Show password in plain text");
    show_cmd->callback([this, show_args]() { handle(*show_args); });

    auto&& delete_args = std::make_shared<console_view::delete_args>();
    auto&& delete_cmd = app.add_subcommand("delete", "Delete password entry by ID");
    delete_cmd->add_option("--id", delete_args->id_, "Entry ID")->required();
    delete_cmd->callback([this, delete_args]() { handle(*delete_args); });

    app.require_subcommand(1);
}

namespace {

auto truncate(std::string_view str, std::size_t max_width) {
    if (str.length() <= max_width) { return std::string{str}; }
    return std::format("{}...", str.substr(0, max_width - 3));
}

auto format_date(pm::domain::time_type timestamp) {
    auto&& time = std::chrono::system_clock::from_time_t(timestamp);
    return std::format("{:%Y-%m-%d}", time);
}

}  // namespace

bool console_view::parse_line(const std::string& line) {
    if (line.empty()) { return true; }

    if (line == "exit" || line == "quit") { return false; }

    auto&& app = CLI::App{};
    setup_commands(app);
    // clang-format off
    try {
        app.parse(line);  
    } catch (const CLI::ParseError& e) {
        std::print(stderr, "{}\n", e.what());
    } catch (const core::pm_error& e) {
        std::print(stderr, "Error: {}\n", e.what());
    }
    // clang-format on
    return true;
}

void console_view::handle(const add_args& args) const {
    auto&& result = service_.add_password(args.title_, args.login_, args.password_);
    std::print(
        "Entry added successfully:\n"
        "  ID: {}\n"
        "  Title: {}\n"
        "  Login: {}\n"
        "  Created: {}\n",
        result.id_, result.title_, result.login_.empty() ? "(empty)" : result.login_, format_date(result.created_at_));
}

void console_view::handle([[maybe_unused]] const list_args& args) const {
    auto&& entries = service_.list_passwords();
    if (entries.empty()) {
        std::print("No entries found.\n");
        return;
    }

    constexpr std::size_t id_width = 4;
    constexpr std::size_t title_width = 30;
    constexpr std::size_t login_width = 20;
    constexpr std::size_t date_width = 10;

    auto&& result = std::string{};
    result += std::format("{:<{}} {:<{}} {:<{}} {:<{}}\n", "ID", id_width, "Title", title_width, "Login", login_width,
                          "Created", date_width);
    result += std::format("{:-<{}} {:-<{}} {:-<{}} {:-<{}}\n", "", id_width, "", title_width, "", login_width, "",
                          date_width);

    for (auto&& entry : entries) {
        result += std::format("{:<{}} {:<{}} {:<{}} {:<{}}\n", entry.id_, id_width, truncate(entry.title_, title_width),
                              title_width, truncate(entry.login_.empty() ? "(empty)" : entry.login_, login_width),
                              login_width, format_date(entry.created_at_), date_width);
    }

    std::print("{}", result);
}

void console_view::handle(const show_args& args) const {
    auto&& result = service_.get_password(args.id_);
    auto&& password_display = args.reveal_ ? result.password_ : std::string(result.password_.size(), '*');
    std::print(
        "Entry details:\n"
        "  ID: {}\n"
        "  Title: {}\n"
        "  Login: {}\n"
        "  Password: {}\n"
        "  Created: {}\n",
        result.id_, result.title_, result.login_.empty() ? "(empty)" : result.login_, password_display,
        format_date(result.created_at_));
}

void console_view::handle(const delete_args& args) const {
    service_.delete_password(args.id_);
    std::print("Entry with ID {} deleted successfully.\n", args.id_);
}

}  // namespace pm::presentation
