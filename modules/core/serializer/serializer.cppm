module;

#include <span>

export module pm.core.serializer;

import pm.core.crypto.types;
import pm.domain;

export namespace pm::core {

class serializer {
public:
    [[nodiscard]] static byte_buffer to_bytes(const domain::password_entry& entry);
    [[nodiscard]] static domain::password_entry from_bytes(std::span<const byte> data);
};

}  // namespace pm::core
