module;

#include <span>
#include <string_view>

export module pm.core.crypto.sodium_provider;

import pm.core.crypto.types;
import pm.core.crypto.key;
import pm.core.crypto.provider;

export namespace pm::core {

class sodium_crypto_provider final : public crypto_provider {
public:
    sodium_crypto_provider();

    [[nodiscard]] crypto_key derive_key(std::string_view master_password, std::span<const byte> salt) const override;
    [[nodiscard]] encrypted_blob encrypt(std::span<const byte> plaintext, const crypto_key& key) const override;
    [[nodiscard]] byte_buffer decrypt(std::span<const byte> blob, const crypto_key& key) const override;
    [[nodiscard]] static byte_buffer generate_salt();
};


}  // namespace pm::core
