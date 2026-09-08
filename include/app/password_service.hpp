#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"

#include <expected>
#include <string>

namespace pm::app {

class password_service {
public:
    password_service(infrastructure::password_repository& repository) : repository_{repository} {}

    [[nodiscard]] std::expected<domain::password_entry, std::string> add_password(std::string title, std::string login,
                                                                                  std::string password);

    [[nodiscard]] domain::password_list list_passwords() const { return repository_.get_all(); }

private:
    infrastructure::password_repository& repository_;
};

}  // namespace pm::app
