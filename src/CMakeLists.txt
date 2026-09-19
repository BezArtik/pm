add_library(password-manager-lib STATIC)

target_sources(password-manager-lib PRIVATE
    app/password_service.cpp
    app/application.cpp
    infrastructure/sqlite_repository.cpp
    presentation/console_view.cpp
    presentation/repl_session.cpp
)

target_include_directories(password-manager-lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(password-manager-lib PUBLIC 
    sqlite_orm::sqlite_orm 
    CLI11::CLI11
)

add_executable(pm main.cpp)
target_link_libraries(pm PRIVATE password-manager-lib)

