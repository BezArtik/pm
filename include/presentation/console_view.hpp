// presentation//console_view.hpp

#pragma once

#include "app/password_service.hpp"

#include <span>
#include <string_view>

namespace pm::presentation {

class console_view {
public:
    console_view(pm::app::password_service& service) : service_{service} {}

    [[nodiscard]] std::int32_t run(std::span<const std::string_view> args) const;

private:
    [[nodiscard]] std::int32_t handle_add(std::span<const std::string_view> args) const;
    [[nodiscard]] std::int32_t handle_list() const;

    pm::app::password_service& service_;
};

}  // namespace pm::presentation
