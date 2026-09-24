module;

#include <cstdint>
#include <format>
#include <span>

module pm.core.serializer;

import pm.core.errors;
import pm.core.utils.serialization;

namespace pm::core {

namespace {

constexpr std::uint8_t format_version = 1;
constexpr std::size_t header_size = u8_size;
constexpr std::size_t fields_count = 3;

void write_string(byte_buffer& out, std::string_view str) {
    write_buffer(out, {str.data(), str.size()});
}

std::string read_string(std::span<const byte> data, std::size_t& offset) {
    auto&& buf = read_buffer(data, offset);
    return {buf.begin(), buf.end()};
}

}  // namespace

byte_buffer serializer::to_bytes(const domain::password_entry& entry) {
    auto&& out = byte_buffer{};
    out.reserve(header_size + (fields_count * u32_size) + entry.title_.size() + entry.login_.size() +
                entry.password_.size());

    out.push_back(to_byte(format_version));
    write_string(out, entry.title_);
    write_string(out, entry.login_);
    write_string(out, entry.password_);

    return out;
}

domain::password_entry serializer::from_bytes(std::span<const byte> data) {
    if (data.empty()) { throw serialization_error{"empty data"}; }

    auto&& version = to_u8(data[0]);
    if (version != format_version) { throw serialization_error{std::format("unsupported version {}", version)}; }

    auto&& offset = std::size_t{u8_size};

    auto&& entry = domain::password_entry{};
    entry.title_ = read_string(data, offset);
    entry.login_ = read_string(data, offset);
    entry.password_ = read_string(data, offset);

    return entry;
}

}  // namespace pm::core
