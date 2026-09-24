module;

#include <sodium.h>

module pm.core.crypto.key;

namespace pm::core {

crypto_key::crypto_key(crypto_key&& other) noexcept : bytes_{other.bytes_}, is_valid_{other.is_valid_} {
    sodium_memzero(other.bytes_.data(), other.bytes_.size());
    other.is_valid_ = false;
}

crypto_key& crypto_key::operator=(crypto_key&& other) noexcept {
    if (this == &other) { return *this; }
    sodium_memzero(bytes_.data(), bytes_.size());
    bytes_ = other.bytes_;
    is_valid_ = other.is_valid_;
    sodium_memzero(other.bytes_.data(), other.bytes_.size());
    other.is_valid_ = false;
    return *this;
}

crypto_key::~crypto_key() {
    sodium_memzero(bytes_.data(), bytes_.size());
}

}  // namespace pm::core
