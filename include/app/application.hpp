#pragma once

#include "app/password_service.hpp"
#include "core/crypto/sodium_provider.hpp"
#include "infrastructure/sqlite_repository.hpp"
#include "infrastructure/vault_storage.hpp"
#include "presentation/console_view.hpp"

namespace pm::app {

class application {
public:
    application()
        : repository_{"passwords.db"},
          vault_storage_{"passwords.vault"},
          service_{repository_, vault_storage_, crypto_},
          view_{service_} {}

    [[nodiscard]] int run(int argc, char** argv);

private:
    void run_repl();
    void run_single_command(int argc, char** argv);

    core::sodium_crypto_provider crypto_;
    infrastructure::sqlite_repository repository_;
    infrastructure::vault_storage vault_storage_;
    password_service service_;
    presentation::console_view view_;
};

}  // namespace pm::app
