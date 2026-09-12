// infrastructure/password_repository.hpp
#pragma once

#include "domain/password.hpp"

#include <optional>

namespace pm::infrastructure {

class password_repository {
public:
    virtual ~password_repository() = default;

    password_repository() = default;

    password_repository(const password_repository&) = delete;
    password_repository& operator=(const password_repository&) = delete;

    password_repository(password_repository&&) = delete;
    password_repository& operator=(password_repository&&) = delete;

    virtual domain::password_entry add(domain::password_entry entry) = 0;
    [[nodiscard]] virtual domain::password_list get_all() const = 0;
    [[nodiscard]] virtual std::optional<domain::password_entry> find_by_id(domain::id_type id) const = 0;
    virtual bool remove(domain::id_type id) = 0;
};

}  // namespace pm::infrastructure
