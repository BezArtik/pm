// infrastructure/sqlite_repository.cpp

#include "infrastructure/sqlite_repository.hpp"

#include "core/errors/error.hpp"
#include "domain/password.hpp"
#include "infrastructure/stored_password.hpp"

#include "sqlite_orm/sqlite_orm.h"
#include <memory>

namespace pm::infrastructure {

using namespace sqlite_orm;

namespace {

auto create_storage(std::string db_path) {
    // clang-format off
    return make_storage(
        std::move(db_path),
        make_table("passwords",
                   make_column("id", &stored_password::id_, primary_key().autoincrement()),
                   make_column("blob", &stored_password::blob_),
                   make_column("created_at", &stored_password::created_at_)));
    // clang-format on
}

}  // namespace

struct sqlite_repository::impl {
    decltype(create_storage("")) storage_;

    impl(std::string db_path) : storage_{create_storage(std::move(db_path))} { storage_.sync_schema(); }
};

sqlite_repository::sqlite_repository(std::string db_path) : pimpl_(std::make_unique<impl>(std::move(db_path))) {}

sqlite_repository::~sqlite_repository() = default;

stored_password sqlite_repository::add(stored_password entry) {
    try {
        entry.id_ = static_cast<domain::id_type>(pimpl_->storage_.insert(entry));
        return entry;
    } catch (const std::exception& e) { throw core::database_error{e.what()}; }
}

stored_password_list sqlite_repository::get_all() const {
    try {
        return pimpl_->storage_.get_all<stored_password>();
    } catch (const std::exception& e) { throw core::database_error{e.what()}; }
}

stored_password sqlite_repository::find_by_id(domain::id_type id) const {
    try {
        auto&& entry = pimpl_->storage_.get_optional<stored_password>(id);
        if (!entry) { throw core::entry_not_found_error{id}; }
        return *entry;
    } catch (const core::pm_error&) { throw; } catch (const std::exception& e) {
        throw core::database_error{e.what()};
    }
}

void sqlite_repository::remove(domain::id_type id) {
    try {
        pimpl_->storage_.transaction([&] {
            pimpl_->storage_.remove<stored_password>(id);
            if (pimpl_->storage_.changes() == 0) { throw core::entry_not_found_error{id}; }
            return true;
        });
        pimpl_->storage_.remove<stored_password>(id);
    } catch (const core::pm_error&) { throw; } catch (const std::exception& e) {
        throw core::database_error{e.what()};
    }
}

}  // namespace pm::infrastructure
