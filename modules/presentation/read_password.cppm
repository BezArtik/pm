module;

#include <string>

export module pm.presentation.read_password;

export namespace pm::presentation {

[[nodiscard]] std::string read_master_password();

}  // namespace pm::presentation
