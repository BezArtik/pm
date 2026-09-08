#include "infrastructure/sqlite_repository.hpp"

#include "domain/password.hpp"

#include <chrono>
#include <format>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <string_view>

namespace pm::infrastructure {

sqlite_repository::sqlite_repository(std::string_view db_path) {
    if (sqlite3_open(db_path.data(), &db_) != SQLITE_OK) {
        throw std::runtime_error{std::format("Cannot open database: {}", sqlite3_errmsg(db_))};
    }
    create_schema();
}

sqlite_repository::~sqlite_repository() {
    if (db_) { sqlite3_close(db_); }
}

void sqlite_repository::create_schema() {
    std::string_view sql = R"(
        CREATE TABLE IF NOT EXISTS passwords (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            login TEXT,
            password TEXT NOT NULL,
            created_at TEXT NOT NULL
        );
    )";

    char* error_message = nullptr;
    if (sqlite3_exec(db_, sql.data(), nullptr, nullptr, &error_message) != SQLITE_OK) {
        std::string error = error_message ? error_message : "Unknown error";
        sqlite3_free(error_message);
        throw std::runtime_error{std::format("Cannot create schema: {}", error)};
    }
}

domain::password_entry sqlite_repository::add(pm::domain::password_entry entry) {
    std::string_view sql = R"(
        INSERT INTO passwords (title, login, password, created_at)
        VALUES (?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error{std::format("Cannot prepare statement: {}", sqlite3_errmsg(db_))};
    }

    sqlite3_bind_text(stmt, 1, entry.title_.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, entry.login_.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, entry.password_.c_str(), -1, SQLITE_TRANSIENT);

    auto created_at = std::format("{:%Y-%m-%d %H:%M:%S}", entry.created_at_);
    sqlite3_bind_text(stmt, 4, created_at.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error{std::format("Cannot insert entry: {}", sqlite3_errmsg(db_))};
    }

    entry.id_ = sqlite3_last_insert_rowid(db_);
    sqlite3_finalize(stmt);
    return entry;
}

domain::password_list sqlite_repository::get_all() const {
    std::string_view sql = R"(
        SELECT id, title, login, password, created_at
        FROM passwords
        ORDER BY created_at DESC;
    )";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.data(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error{std::format("Cannot prepare statement: {}", sqlite3_errmsg(db_))};
    }

    domain::password_list entries;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        pm::domain::password_entry entry;
        entry.id_ = sqlite3_column_int64(stmt, 0);

        auto&& title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        entry.title_ = title ? title : "";

        auto&& login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        entry.login_ = login ? login : "";

        auto&& password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        entry.password_ = password ? password : "";

        auto&& created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        if (created_at) {
            std::tm time{};
            std::istringstream stream{created_at};
            stream >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");
            entry.created_at_ = std::chrono::system_clock::from_time_t(std::mktime(&time));
        } else {
            entry.created_at_ = std::chrono::system_clock::now();
        }
        entries.push_back(std::move(entry));
    }

    sqlite3_finalize(stmt);
    return entries;
}

}  // namespace pm::infrastructure
