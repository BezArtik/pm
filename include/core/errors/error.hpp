// core/errors/error.hpp
#pragma once

#include "domain/password.hpp"

#include <format>
#include <stdexcept>
#include <string>

namespace pm::core {

struct pm_error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct empty_title_error : pm_error {
    empty_title_error() noexcept : pm_error{"Title cannot be empty"} {}
};

struct empty_password_error : pm_error {
    empty_password_error() noexcept : pm_error{"Password cannot be empty"} {}
};

struct entry_not_found_error : pm_error {
    entry_not_found_error(domain::id_type id) : pm_error{std::format("Entry with id {} not found", id)} {}
};

struct database_error : pm_error {
    database_error(std::string details) : pm_error{std::format("Database error: {}", details)} {}
};

}  // namespace pm::core
