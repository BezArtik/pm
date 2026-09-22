// app/password_service.cpp

#include "app/password_service.hpp"

#include "core/crypto/sodium_provider.hpp"
#include "core/errors/error.hpp"
#include "core/serializer/serializer.hpp"

#include <chrono>
#include <utility>

namespace pm::app {

namespace {

constexpr std::string_view verifier_plaintext = "PM_VAULT_CHECK";

}  // namespace

void password_service::unlock(std::string_view master_password) {
    auto&& header = vault_storage_.load();

    if (!header) {
        auto&& salt = core::sodium_crypto_provider::generate_salt();
        auto&& key = crypto_.derive_key(master_password, salt);
        auto&& verifier = crypto_.encrypt({verifier_plaintext.data(), verifier_plaintext.size()}, key);
        vault_storage_.save({std::move(salt), std::move(verifier)});
        key_ = std::move(key);
        return;
    }

    auto&& key = crypto_.derive_key(master_password, header->salt_);
    auto&& decrypted = crypto_.decrypt(header->verifier_, key);

    key_ = std::move(key);
}

void password_service::require_unlocked() const {
    if (!key_) { throw core::crypto_error{"Vault is locked"}; }
}

domain::password_entry password_service::add_password(std::string title, std::string login, std::string password) {
    require_unlocked();

    if (title.empty()) { throw core::empty_title_error{}; }
    if (password.empty()) { throw core::empty_password_error{}; }

    auto&& now = std::chrono::system_clock::now();
    auto&& timestamp = static_cast<domain::time_type>(
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());

    domain::password_entry entry{.title_ = std::move(title),
                                 .login_ = std::move(login),
                                 .password_ = std::move(password),
                                 .created_at_ = timestamp};

    auto&& serialized = core::serializer::to_bytes(entry);
    auto&& blob = crypto_.encrypt(serialized, *key_);

    auto&& stored = repo_.add({0, std::move(blob), timestamp});

    entry.id_ = stored.id_;
    return entry;
}

domain::password_list password_service::list_passwords() const {
    require_unlocked();

    auto&& stored_list = repo_.get_all();
    domain::password_list result;
    result.reserve(stored_list.size());

    for (auto&& stored : stored_list) {
        auto&& plaintext = crypto_.decrypt(stored.blob_, *key_);
        auto&& entry = core::serializer::from_bytes(plaintext);
        entry.id_ = stored.id_;
        entry.created_at_ = stored.created_at_;
        result.push_back(std::move(entry));
    }

    return result;
}

domain::password_entry password_service::get_password(domain::id_type id) const {
    require_unlocked();

    auto&& stored = repo_.find_by_id(id);
    auto&& plaintext = crypto_.decrypt(stored.blob_, *key_);
    auto&& entry = core::serializer::from_bytes(plaintext);
    entry.id_ = stored.id_;
    entry.created_at_ = stored.created_at_;
    return entry;
}

void password_service::delete_password(domain::id_type id) {
    require_unlocked();
    repo_.remove(id);
}

}  // namespace pm::app
