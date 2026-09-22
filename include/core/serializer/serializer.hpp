// core/serializer/serializer.hpp
#pragma once

#include "core/crypto/types.hpp"
#include "domain/password.hpp"

#include <span>

namespace pm::core {

class serializer {
public:
    [[nodiscard]] static byte_buffer to_bytes(const domain::password_entry& entry);
    [[nodiscard]] static domain::password_entry from_bytes(std::span<const byte> data);
};

}  // namespace pm::core
