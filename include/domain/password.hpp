// domain/password.hpp

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace pm::domain {

using id_type = std::uint64_t;
using time_type = std::int64_t;

struct password_entry {
    id_type id_{0};
    std::string title_;
    std::string login_;
    std::string password_;
    time_type created_at_;
};

using password_list = std::vector<password_entry>;

}  // namespace pm::domain
