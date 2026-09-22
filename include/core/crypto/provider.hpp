// include/core/crypto/provider.hpp
#pragma once

#include "core/crypto/key.hpp"
#include "core/crypto/types.hpp"

#include <span>
#include <string_view>

namespace pm::core {

class crypto_provider {
public:
    virtual ~crypto_provider() = default;

    crypto_provider() = default;
    crypto_provider(const crypto_provider&) = delete;
    crypto_provider& operator=(const crypto_provider&) = delete;
    crypto_provider(crypto_provider&&) = delete;
    crypto_provider& operator=(crypto_provider&&) = delete;

    [[nodiscard]] virtual crypto_key derive_key(std::string_view master_password, std::span<const byte> salt) const = 0;
    [[nodiscard]] virtual encrypted_blob encrypt(std::span<const byte> plaintext, const crypto_key& key) const = 0;
    [[nodiscard]] virtual byte_buffer decrypt(std::span<const byte> blob, const crypto_key& key) const = 0;
};

}  // namespace pm::core
