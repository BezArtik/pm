// infrastructure/vault_storage.hpp
#pragma once

#include "core/crypto/types.hpp"

#include <filesystem>
#include <optional>

namespace pm::infrastructure {

class vault_storage {
    struct vault_header {
        core::byte_buffer salt_;
        core::byte_buffer verifier_;
    };

public:
    vault_storage(std::filesystem::path path) : path_{std::move(path)} {}

    [[nodiscard]] std::optional<vault_header> load() const;
    void save(const vault_header& header) const;

private:
    std::filesystem::path path_;
};

}  // namespace pm::infrastructure
