#ifndef IRCT_CHECKSUM_INDEX_H
#define IRCT_CHECKSUM_INDEX_H
#include <cstddef>
#include <cstdint>
#include <vector>
namespace irct {
class ChecksumIndex {
public:
    ChecksumIndex();
    void add_block(const uint8_t *data, size_t size);
    uint32_t block_checksum(size_t i) const;
    size_t block_count() const;
    uint32_t rolling() const;
    bool verify(size_t i, const uint8_t *data, size_t size) const;
    void reset();
    uint32_t combined() const;
private:
    std::vector<uint32_t> checksums_;
    std::vector<size_t> sizes_;
};
}
#endif
