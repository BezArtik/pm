#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace pm::domain {

struct password_entry {
    std::uint64_t id_{0};
    std::string title_;
    std::string login_;
    std::string password_;
    std::chrono::system_clock::time_point created_at_;
};

using password_list = std::vector<password_entry>;

}  // namespace pm::domain
