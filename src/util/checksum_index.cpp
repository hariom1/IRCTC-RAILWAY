#include "irct/checksum_index.h"
#include "irct/crc32.h"
namespace irct {
ChecksumIndex::ChecksumIndex() {}
void ChecksumIndex::add_block(const uint8_t *data, size_t size) {
    uint32_t c = Crc32::compute(data, size);
    checksums_.push_back(c);
    sizes_.push_back(size);
}
uint32_t ChecksumIndex::block_checksum(size_t i) const {
    if (i >= checksums_.size()) {
        return 0;
    }
    return checksums_[i];
}
size_t ChecksumIndex::block_count() const { return checksums_.size(); }
uint32_t ChecksumIndex::rolling() const {
    uint32_t acc = 0;
    for (size_t i = 0; i < checksums_.size(); ++i) {
        acc = (acc << 1) ^ checksums_[i];
    }
    return acc;
}
bool ChecksumIndex::verify(size_t i, const uint8_t *data, size_t size) const {
    if (i >= checksums_.size()) {
        return false;
    }
    if (sizes_[i] != size) {
        return false;
    }
    return Crc32::compute(data, size) == checksums_[i];
}
void ChecksumIndex::reset() {
    checksums_.clear();
    sizes_.clear();
}
uint32_t ChecksumIndex::combined() const {
    Crc32 c;
    for (size_t i = 0; i < checksums_.size(); ++i) {
        uint8_t bytes[4];
        bytes[0] = static_cast<uint8_t>(checksums_[i] & 0xFFu);
        bytes[1] = static_cast<uint8_t>((checksums_[i] >> 8) & 0xFFu);
        bytes[2] = static_cast<uint8_t>((checksums_[i] >> 16) & 0xFFu);
        bytes[3] = static_cast<uint8_t>((checksums_[i] >> 24) & 0xFFu);
        c.update(bytes, 4);
    }
    return c.value();
}
}
