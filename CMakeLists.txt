cmake_minimum_required(VERSION 3.30...4.1)
project(password_manager LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    sqlite_orm
    GIT_REPOSITORY https://github.com/fnc12/sqlite_orm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(sqlite_orm)

set(CLI11_MODULES ON CACHE BOOL "Enable CLI11 C++20 modules" FORCE)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(
    Sodium
    GIT_REPOSITORY https://github.com/zerokol-cmd/libsodium-cmake-latest.git
    GIT_TAG 280aab4ed68510a58877a529a4f0ffbd66bf7749
)
set(SODIUM_DISABLE_TESTS ON)
FetchContent_MakeAvailable(Sodium)

add_subdirectory(src)

