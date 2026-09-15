// app/password_service.hpp
#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"

#include <string>

namespace pm::app {

class password_service {
public:
    password_service(infrastructure::password_repository& repository) : repository_{repository} {}

    ~password_service() = default;
    password_service(const password_service&) = delete;
    password_service& operator=(const password_service&) = delete;
    password_service(password_service&&) = delete;
    password_service& operator=(password_service&&) = delete;

    domain::password_entry add_password(std::string title, std::string login, std::string password);
    [[nodiscard]] domain::password_list list_passwords() const;
    [[nodiscard]] domain::password_entry get_password(domain::id_type id) const;
    void delete_password(domain::id_type id);

private:
    infrastructure::password_repository& repository_;
};

}  // namespace pm::app
