// app/password_service.cpp

#include "app/password_service.hpp"

#include "domain/password.hpp"

#include <chrono>
#include <string>

namespace pm::app {

std::expected<pm::domain::password_entry, std::string> password_service::add_password(std::string title,
                                                                                      std::string login,
                                                                                      std::string password) {
    if (title.empty()) { return std::unexpected{"title cannot be empty"}; }

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    pm::domain::password_entry entry{.title_ = std::move(title),
                                     .login_ = std::move(login),
                                     .password_ = std::move(password),
                                     .created_at_ = timestamp};

    entry = repository_.add(std::move(entry));
    return entry;
}

std::expected<domain::password_entry, std::string> password_service::get_password(domain::id_type id) const {
    auto entry = repository_.find_by_id(id);

    if (!entry) { return std::unexpected{"Entry not found"}; }

    return *entry;
}

std::expected<void, std::string> password_service::delete_password(domain::id_type id) {
    if (!repository_.remove(id)) { return std::unexpected{"Entry not found"}; }
    return {};
}

}  // namespace pm::app
