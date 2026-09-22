#pragma once

#include "core/crypto/types.hpp"

#include <cstdint>
#include <span>

namespace pm::core {

constexpr std::size_t u32_size = 4;
constexpr std::size_t u8_size = 1;

constexpr std::uint32_t byte_mask = 0xFF;
constexpr unsigned shift_0 = 0;
constexpr unsigned shift_1 = 8;
constexpr unsigned shift_2 = 16;
constexpr unsigned shift_3 = 24;

[[nodiscard]] inline std::uint8_t to_u8(byte b) noexcept {
    return static_cast<std::uint8_t>(static_cast<unsigned char>(b));
}

[[nodiscard]] inline byte to_byte(std::uint8_t v) noexcept {
    return static_cast<byte>(v);
}

void write_u32_be(byte_buffer& out, std::uint32_t value);
[[nodiscard]] std::uint32_t read_u32_be(std::span<const byte> data, std::size_t offset);

void write_buffer(byte_buffer& out, std::span<const byte> buf);
[[nodiscard]] byte_buffer read_buffer(std::span<const byte> data, std::size_t& offset);

}  // namespace pm::core
