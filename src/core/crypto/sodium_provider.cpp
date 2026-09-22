#include "core/crypto/sodium_provider.hpp"

#include "core/errors/error.hpp"

#include <array>
#include <sodium.h>

namespace pm::core {

namespace {

constexpr std::size_t salt_size = crypto_pwhash_SALTBYTES;
constexpr std::size_t nonce_size = crypto_secretbox_NONCEBYTES;
constexpr std::size_t mac_size = crypto_secretbox_MACBYTES;

unsigned char* as_uchar(byte* p) noexcept {
    return reinterpret_cast<unsigned char*>(p);
}

const unsigned char* as_uchar(const byte* p) noexcept {
    return reinterpret_cast<const unsigned char*>(p);
}

}  // namespace

byte_buffer sodium_crypto_provider::generate_salt() {
    auto&& salt = byte_buffer(salt_size);
    randombytes_buf(salt.data(), salt.size());
    return salt;
}

sodium_crypto_provider::sodium_crypto_provider() {
    if (sodium_init() < 0) { throw crypto_error{"Failed to initialize libsodium"}; }
}

crypto_key sodium_crypto_provider::derive_key(std::string_view master_password, std::span<const byte> salt) const {
    if (salt.size() != salt_size) { throw crypto_error{"Invalid salt size"}; }

    auto&& key = crypto_key::bytes_t{};

    if (crypto_pwhash(as_uchar(key.data()), key.size(), master_password.data(), master_password.size(),
                      as_uchar(salt.data()), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw crypto_error{"Key derivation failed"};
    }

    return key;
}

encrypted_blob sodium_crypto_provider::encrypt(std::span<const byte> plaintext, const crypto_key& key) const {
    if (!key.is_valid()) { throw crypto_error{"Invalid key"}; }

    auto&& nonce = std::array<byte, nonce_size>{};
    randombytes_buf(nonce.data(), nonce.size());

    auto&& result = encrypted_blob(mac_size + plaintext.size());

    if (crypto_secretbox_easy(as_uchar(result.data()), as_uchar(plaintext.data()), plaintext.size(),
                              as_uchar(nonce.data()), as_uchar(key.raw_bytes())) != 0) {
        throw crypto_error{"Encryption failed"};
    }

    result.insert(result.end(), nonce.begin(), nonce.end());
    return result;
}

byte_buffer sodium_crypto_provider::decrypt(std::span<const byte> blob, const crypto_key& key) const {
    if (!key.is_valid()) { throw invalid_master_password_error{}; }

    if (blob.size() < mac_size + nonce_size) { throw crypto_error{"Blob too small"}; }

    auto&& ciphertext_size = blob.size() - nonce_size;
    auto&& plaintext = byte_buffer(ciphertext_size - mac_size);

    if (crypto_secretbox_open_easy(as_uchar(plaintext.data()), as_uchar(blob.data()), ciphertext_size,
                                   as_uchar(blob.data() + ciphertext_size), as_uchar(key.raw_bytes())) != 0) {
        throw invalid_master_password_error{};
    }

    return plaintext;
}

}  // namespace pm::core
