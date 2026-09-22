// infrastructure/sqlite_repository.hpp
#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"
#include "infrastructure/stored_password.hpp"

#include <memory>
#include <string>

namespace pm::infrastructure {

class sqlite_repository final : public password_repository {
public:
    sqlite_repository(std::string db_path);

    ~sqlite_repository() override;
    sqlite_repository(const sqlite_repository&) = delete;
    sqlite_repository& operator=(const sqlite_repository&) = delete;
    sqlite_repository(sqlite_repository&&) = delete;
    sqlite_repository& operator=(sqlite_repository&&) = delete;

    stored_password add(stored_password entry) override;
    [[nodiscard]] stored_password_list get_all() const override;
    [[nodiscard]] stored_password find_by_id(domain::id_type id) const override;
    void remove(domain::id_type id) override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}  // namespace pm::infrastructure
