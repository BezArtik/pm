module;

#include <cstdint>
#include <span>

module pm.core.utils.serialization;

import pm.core.errors;

namespace pm::core {

void write_u32_be(byte_buffer& out, std::uint32_t value) {
    out.push_back(to_byte((value >> shift_3) & byte_mask));
    out.push_back(to_byte((value >> shift_2) & byte_mask));
    out.push_back(to_byte((value >> shift_1) & byte_mask));
    out.push_back(to_byte((value >> shift_0) & byte_mask));
}

std::uint32_t read_u32_be(std::span<const byte> data, std::size_t offset) {
    auto&& b0 = static_cast<std::uint32_t>(to_u8(data[offset + 0]));
    auto&& b1 = static_cast<std::uint32_t>(to_u8(data[offset + 1]));
    auto&& b2 = static_cast<std::uint32_t>(to_u8(data[offset + 2]));
    auto&& b3 = static_cast<std::uint32_t>(to_u8(data[offset + 3]));
    return (b0 << shift_3) | (b1 << shift_2) | (b2 << shift_1) | (b3 << shift_0);
}

void write_buffer(byte_buffer& out, std::span<const byte> buf) {
    write_u32_be(out, static_cast<std::uint32_t>(buf.size()));
    out.insert(out.end(), buf.begin(), buf.end());
}

byte_buffer read_buffer(std::span<const byte> data, std::size_t& offset) {
    if (offset + u32_size > data.size()) { throw serialization_error{"unexpected end of data (length prefix)"}; }

    auto&& len = read_u32_be(data, offset);
    offset += u32_size;

    if (offset + len > data.size()) { throw serialization_error{"buffer length exceeds data size"}; }

    auto&& buf = byte_buffer(data.begin() + static_cast<std::ptrdiff_t>(offset),
                             data.begin() + static_cast<std::ptrdiff_t>(offset + len));
    offset += len;
    return buf;
}

}  // namespace pm::core
