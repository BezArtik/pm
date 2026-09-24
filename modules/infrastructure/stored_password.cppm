module;

#include <vector>

export module pm.infrastructure.stored_password;

import pm.core.crypto.types;
import pm.domain;

export namespace pm::infrastructure {

struct stored_password {
    domain::id_type id_{0};
    core::encrypted_blob blob_;
    domain::time_type created_at_{0};
};

using stored_password_list = std::vector<stored_password>;

}  // namespace pm::infrastructure

