cmake_minimum_required(VERSION 3.30...4.1)
project(password_manager LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    sqlite_orm
    GIT_REPOSITORY https://github.com/fnc12/sqlite_orm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(sqlite_orm)

add_subdirectory(src)

