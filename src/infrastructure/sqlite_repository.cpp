// infrastructure/sqlite_repository.cpp

#include "infrastructure/sqlite_repository.hpp"

#include "core/errors/error.hpp"
#include "domain/password.hpp"

#include "sqlite_orm/sqlite_orm.h"
#include <memory>

namespace pm::infrastructure {

namespace {

auto create_storage(std::string db_path) {
    using namespace sqlite_orm;
    // clang-format off
    return make_storage(
        std::move(db_path),
        make_table("passwords", 
                   make_column("id", &domain::password_entry::id_, primary_key().autoincrement()),
                   make_column("title", &domain::password_entry::title_),
                   make_column("login", &domain::password_entry::login_),
                   make_column("password", &domain::password_entry::password_),
                   make_column("created_at", &domain::password_entry::created_at_)));
    // clang-format on
}

}  // namespace

struct sqlite_repository::impl {
    using storage_type = decltype(create_storage(""));

    storage_type storage_;

    impl(std::string db_path) : storage_{create_storage(std::move(db_path))} { storage_.sync_schema(); }
};

sqlite_repository::sqlite_repository(std::string db_path) : pimpl_(std::make_unique<impl>(std::move(db_path))) {}

sqlite_repository::~sqlite_repository() = default;

domain::password_entry sqlite_repository::add(domain::password_entry entry) {
    try {
        entry.id_ = static_cast<domain::id_type>(pimpl_->storage_.insert(entry));
        return entry;
    } catch (const std::exception& e) { throw core::database_error{e.what()}; }
}

domain::password_list sqlite_repository::get_all() const {
    try {
        return pimpl_->storage_.get_all<domain::password_entry>();
    } catch (const std::exception& e) { throw core::database_error{e.what()}; }
}

domain::password_entry sqlite_repository::find_by_id(domain::id_type id) const {
    try {
        auto entry = pimpl_->storage_.get_optional<domain::password_entry>(id);
        if (!entry) { throw core::entry_not_found_error{id}; }
        return *entry;
    } catch (const core::pm_error&) { throw; } catch (const std::exception& e) {
        throw core::database_error{e.what()};
    }
}

void sqlite_repository::remove(domain::id_type id) {
    try {
        pimpl_->storage_.transaction([&] {
            pimpl_->storage_.remove<domain::password_entry>(id);
            if (pimpl_->storage_.changes() == 0) { throw core::entry_not_found_error{id}; }
            return true;
        });
        pimpl_->storage_.remove<domain::password_entry>(id);
    } catch (const core::pm_error&) { throw; } catch (const std::exception& e) {
        throw core::database_error{e.what()};
    }
}

}  // namespace pm::infrastructure
