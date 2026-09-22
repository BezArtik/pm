// app/password_service.hpp
#pragma once

#include "core/crypto/key.hpp"
#include "core/crypto/provider.hpp"
#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"
#include "infrastructure/vault_storage.hpp"

#include <optional>
#include <string>

namespace pm::app {

class password_service {
public:
    password_service(infrastructure::password_repository& repo, infrastructure::vault_storage& vault_storage,
                     core::crypto_provider& crypto)
        : repo_{repo}, vault_storage_{vault_storage}, crypto_{crypto} {}

    password_service() = delete;
    ~password_service() = default;
    password_service(const password_service&) = delete;
    password_service& operator=(const password_service&) = delete;
    password_service(password_service&&) = delete;
    password_service& operator=(password_service&&) = delete;

    domain::password_entry add_password(std::string title, std::string login, std::string password);
    [[nodiscard]] domain::password_list list_passwords() const;
    [[nodiscard]] domain::password_entry get_password(domain::id_type id) const;
    void delete_password(domain::id_type id);

    void unlock(std::string_view master_password);
    [[nodiscard]] bool is_unlocked() const noexcept { return key_.has_value(); }

private:
    void require_unlocked() const;

    infrastructure::password_repository& repo_;
    infrastructure::vault_storage& vault_storage_;
    core::crypto_provider& crypto_;
    std::optional<core::crypto_key> key_;
};

}  // namespace pm::app
