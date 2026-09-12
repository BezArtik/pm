// app/password_service.hpp
#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"

#include <expected>
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

    [[nodiscard]] std::expected<domain::password_entry, std::string> add_password(std::string title, std::string login,
                                                                                  std::string password);

    [[nodiscard]] domain::password_list list_passwords() const { return repository_.get_all(); }

    [[nodiscard]] std::expected<domain::password_entry, std::string> get_password(domain::id_type id) const;

    [[nodiscard]] std::expected<void, std::string> delete_password(domain::id_type id);

private:
    infrastructure::password_repository& repository_;
};

}  // namespace pm::app
