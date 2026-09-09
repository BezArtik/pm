#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"

#include <memory>
#include <string>

namespace pm::infrastructure {

class sqlite_repository : public password_repository {
public:
    sqlite_repository(std::string db_path);

    ~sqlite_repository() override;

    sqlite_repository(const sqlite_repository&) = delete;
    sqlite_repository& operator=(const sqlite_repository&) = delete;

    sqlite_repository(sqlite_repository&&) = delete;
    sqlite_repository& operator=(sqlite_repository&&) = delete;

    domain::password_entry add(domain::password_entry entry) override;
    [[nodiscard]] domain::password_list get_all() const override;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

}  // namespace pm::infrastructure
