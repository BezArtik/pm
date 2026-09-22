#include "infrastructure/vault_storage.hpp"

#include "core/errors/error.hpp"
#include "core/utils/serialization.hpp"

#include <fstream>
#include <iterator>

namespace pm::infrastructure {

namespace {

constexpr std::uint8_t format_version = 1;

}  // namespace

std::optional<vault_storage::vault_header> vault_storage::load() const {
    if (!std::filesystem::exists(path_)) { return std::nullopt; }

    auto&& file = std::ifstream{path_, std::ios::binary};
    if (!file) { throw core::vault_corrupted_error{"cannot open vault header"}; }

    auto&& data = core::byte_buffer{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};

    if (data.empty()) { throw core::vault_corrupted_error{"empty vault header"}; }

    auto&& offset = std::size_t{0};
    auto&& version = core::to_u8(data[offset++]);
    if (version != format_version) { throw core::vault_corrupted_error{"unsupported vault header version"}; }

    try {
        auto&& header = vault_header{};
        header.salt_ = core::read_buffer(data, offset);
        header.verifier_ = core::read_buffer(data, offset);
        return header;
    } catch (const core::serialization_error& e) { throw core::vault_corrupted_error{e.what()}; }
}

void vault_storage::save(const vault_header& header) const {
    auto&& data = core::byte_buffer{};
    data.reserve(core::u8_size + (2 * core::u32_size) + header.salt_.size() + header.verifier_.size());

    data.push_back(core::to_byte(format_version));
    core::write_buffer(data, header.salt_);
    core::write_buffer(data, header.verifier_);

    auto&& file = std::ofstream{path_, std::ios::binary | std::ios::trunc};
    if (!file) { throw core::vault_corrupted_error{"cannot write vault header"}; }

    file.write(data.data(), static_cast<std::streamsize>(data.size()));
}

}  // namespace pm::infrastructure
