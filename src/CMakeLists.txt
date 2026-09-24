set(MODULES_DIR ${CMAKE_SOURCE_DIR}/modules)

add_library(password-manager-lib STATIC)

target_sources(password-manager-lib
    PRIVATE
        core/crypto/key.cpp
        core/crypto/sodium_provider.cpp
        core/utils/serialization.cpp
        core/serializer/serializer.cpp
        infrastructure/sqlite_repository.cpp
        infrastructure/vault_storage.cpp
        app/password_service.cpp
        app/application.cpp
        presentation/read_password.cpp
        presentation/console_view.cpp
        presentation/repl_session.cpp
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${MODULES_DIR}
        FILES
            ${MODULES_DIR}/core/crypto/types.cppm
            ${MODULES_DIR}/core/errors/error.cppm
            ${MODULES_DIR}/core/crypto/key.cppm
            ${MODULES_DIR}/core/crypto/provider.cppm
            ${MODULES_DIR}/core/crypto/sodium_provider.cppm
            ${MODULES_DIR}/core/utils/serialization.cppm
            ${MODULES_DIR}/core/serializer/serializer.cppm
            ${MODULES_DIR}/domain/password.cppm
            ${MODULES_DIR}/infrastructure/stored_password.cppm
            ${MODULES_DIR}/infrastructure/password_repository.cppm
            ${MODULES_DIR}/infrastructure/sqlite_repository.cppm
            ${MODULES_DIR}/infrastructure/vault_storage.cppm
            ${MODULES_DIR}/app/password_service.cppm
            ${MODULES_DIR}/app/application.cppm
            ${MODULES_DIR}/presentation/read_password.cppm
            ${MODULES_DIR}/presentation/console_view.cppm
            ${MODULES_DIR}/presentation/repl_session.cppm
)

target_link_libraries(password-manager-lib PUBLIC
    sqlite_orm::sqlite_orm
    CLI11::Module
    sodium
)

add_executable(pm main.cpp)
target_link_libraries(pm PRIVATE password-manager-lib)
