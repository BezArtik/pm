module;

#include <vector>

export module pm.core.crypto.types;

export namespace pm::core {

using byte = char;
using byte_buffer = std::vector<byte>;
using encrypted_blob = std::vector<byte>;

}  // namespace pm::core
