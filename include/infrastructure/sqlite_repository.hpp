#pragma once

#include "domain/password.hpp"
#include "infrastructure/password_repository.hpp"

#include <string_view>

struct sqlite3;

namespace pm::infrastructure {

class sqlite_repository : public password_repository {
public:
    sqlite_repository(std::string_view db_path);
    ~sqlite_repository() override;

    sqlite_repository(const sqlite_repository&) = delete;
    sqlite_repository& operator=(const sqlite_repository&) = delete;

    sqlite_repository(sqlite_repository&&) = delete;
    sqlite_repository& operator=(sqlite_repository&&) = delete;

    domain::password_entry add(pm::domain::password_entry entry) override;
    [[nodiscard]] domain::password_list get_all() const override;

private:
    sqlite3* db_{nullptr};
    void create_schema();
};

}  // namespace pm::infrastructure
