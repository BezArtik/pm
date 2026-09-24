module;

#include <array>
#include <cstddef>

export module pm.core.crypto.key;

import pm.core.crypto.types;

export namespace pm::core {

class crypto_key {
public:
    static constexpr std::size_t key_size = 32;
    using bytes_t = std::array<byte, key_size>;

    crypto_key() = default;
    crypto_key(const bytes_t& bytes) : bytes_{bytes}, is_valid_{true} {}

    crypto_key(const crypto_key&) = delete;
    crypto_key& operator=(const crypto_key&) = delete;

    crypto_key(crypto_key&& other) noexcept;
    crypto_key& operator=(crypto_key&& other) noexcept;

    ~crypto_key();

    [[nodiscard]] auto raw_bytes() const noexcept { return bytes_.data(); }
    [[nodiscard]] bool is_valid() const noexcept { return is_valid_; }

private:
    bytes_t bytes_{};
    bool is_valid_{false};
};

}  // namespace pm::core
