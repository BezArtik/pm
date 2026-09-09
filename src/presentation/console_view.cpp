#include "presentation/console_view.hpp"

#include "domain/password.hpp"

#include <array>
#include <chrono>
#include <format>
#include <print>

namespace pm::presentation {

namespace {
// clang-format off
constexpr std::array commands = {
    std::string_view{"add"}, 
    std::string_view{"list"}
};

constexpr std::array add_option_names = {
    std::string_view{"--title="}, 
    std::string_view{"--login="},
    std::string_view{"--password="}
};
// clang-format on

std::string truncate(std::string_view str, std::size_t max_width) {
    if (str.length() <= max_width) { return std::string{str}; }
    return std::format("{}...", str.substr(0, max_width - 3));
}

std::string format_date(domain::time_type timestamp) {
    auto time = std::chrono::system_clock::from_time_t(timestamp);
    return std::format("{:%Y-%m-%d}", time);
}

std::string format_add(const pm::domain::password_entry& entry) {
    return std::format(
        "Entry added successfully:\n"
        "  ID: {}\n"
        "  Title: {}\n"
        "  Login: {}\n"
        "  Created: {}\n",
        entry.id_, entry.title_, entry.login_.empty() ? "(empty)" : entry.login_, format_date(entry.created_at_));
}

std::string format_list(std::span<const pm::domain::password_entry> entries) {
    if (entries.empty()) { return "No entries found.\n"; }

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

}  // namespace

std::int32_t console_view::run(std::span<const std::string_view> args) const {
    if (args.empty()) {
        std::print("Error: no command provided\n");
        return 1;
    }

    const auto command = args[0];

    if (command == commands[0]) { return handle_add(args.subspan(1)); }

    if (command == commands[1]) { return handle_list(); }

    std::print("Error: unknown command '{}'\n", command);
    return 1;
}

std::int32_t console_view::handle_add(std::span<const std::string_view> args) const {
    std::string title;
    std::string login;
    std::string password;

    for (const auto arg : args) {
        if (arg.starts_with(add_option_names[0])) {
            title = std::string{arg.substr(add_option_names[0].length())};
        } else if (arg.starts_with(add_option_names[1])) {
            login = std::string{arg.substr(add_option_names[1].length())};
        } else if (arg.starts_with(add_option_names[2])) {
            password = std::string{arg.substr(add_option_names[2].length())};
        }
    }

    if (title.empty()) {
        std::print("Error: --title is required\n");
        return 1;
    }

    auto result = service_.add_password(std::move(title), std::move(login), std::move(password));

    if (!result) {
        std::print("Error: {}\n", result.error());
        return 1;
    }

    std::print("{}", format_add(*result));
    return 0;
}

std::int32_t console_view::handle_list() const {
    std::print("{}", format_list(service_.list_passwords()));
    return 0;
}

}  // namespace pm::presentation
