module;

export module pm.infrastructure.repository;

import pm.core.crypto.types;
import pm.domain;
import pm.infrastructure.stored_password;

export namespace pm::infrastructure {

class password_repository {
public:
    virtual ~password_repository() = default;
    password_repository() = default;
    password_repository(const password_repository&) = delete;
    password_repository& operator=(const password_repository&) = delete;
    password_repository(password_repository&&) = delete;
    password_repository& operator=(password_repository&&) = delete;

    virtual stored_password add(stored_password entry) = 0;
    [[nodiscard]] virtual stored_password_list get_all() const = 0;
    [[nodiscard]] virtual stored_password find_by_id(domain::id_type id) const = 0;
    virtual void remove(domain::id_type id) = 0;
};

}  // namespace pm::infrastructure
