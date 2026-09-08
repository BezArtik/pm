// app/password_service.cpp

#include "app/password_service.hpp"

#include "domain/password.hpp"

#include <string>

namespace pm::app {

std::expected<pm::domain::password_entry, std::string> password_service::add_password(std::string title,
                                                                                      std::string login,
                                                                                      std::string password) {
    if (title.empty()) { return std::unexpected("title cannot be empty"); }

    pm::domain::password_entry entry{.title_ = std::move(title),
                                     .login_ = std::move(login),
                                     .password_ = std::move(password),
                                     .created_at_ = std::chrono::system_clock::now()};

    entry = repository_.add(std::move(entry));
    return entry;
}

}  // namespace pm::app
