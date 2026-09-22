// include/core/crypto/types.hpp
#pragma once

#include <vector>

namespace pm::core {

using byte = char;
using byte_buffer = std::vector<byte>;
using encrypted_blob = std::vector<byte>;

}  // namespace pm::core
