add_library(password-manager-lib STATIC)

target_sources(password-manager-lib PRIVATE
    app/application.cpp
    app/password_service.cpp
    core/serializer/serializer.cpp
    core/crypto/key.cpp
    core/crypto/sodium_provider.cpp
    core/utils/serialization.cpp
    infrastructure/vault_storage.cpp
    infrastructure/sqlite_repository.cpp
    presentation/console_view.cpp
    presentation/read_password.cpp
    presentation/repl_session.cpp
)

target_include_directories(password-manager-lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(password-manager-lib PUBLIC 
    sqlite_orm::sqlite_orm 
    CLI11::CLI11
    sodium
)

add_executable(pm main.cpp)
target_link_libraries(pm PRIVATE password-manager-lib)

