// app/password_service.cpp

#include "app/password_service.hpp"

#include "core/errors/error.hpp"
#include "domain/password.hpp"

#include <chrono>
#include <string>

namespace pm::app {

domain::password_entry password_service::add_password(std::string title, std::string login, std::string password) {
    if (title.empty()) { throw core::empty_title_error{}; }
    if (password.empty()) { throw core::empty_password_error{}; }

    auto now = std::chrono::system_clock::now();
    auto timestamp = static_cast<domain::time_type>(
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());

    domain::password_entry entry{.title_ = std::move(title),
                                 .login_ = std::move(login),
                                 .password_ = std::move(password),
                                 .created_at_ = timestamp};

    return repository_.add(std::move(entry));
}

domain::password_list password_service::list_passwords() const {
    return repository_.get_all();
}

domain::password_entry password_service::get_password(domain::id_type id) const {
    return repository_.find_by_id(id);
}

void password_service::delete_password(domain::id_type id) {
    repository_.remove(id);
}

}  // namespace pm::app
