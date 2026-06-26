#ifndef IRCT_PACKED_INDEX_H
#define IRCT_PACKED_INDEX_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "irct/station_index.h"

namespace irct {

struct PackedHeader {
    uint32_t magic;
    uint32_t entry_count;
    uint32_t blob_len;
    uint32_t checksum;
};

class PackedIndex {
public:
    PackedIndex();

    std::vector<uint8_t> serialize(const StationIndex &index) const;

    bool deserialize(const uint8_t *data, size_t size);

    size_t entry_count() const;

    bool find(const std::string &code, uint32_t *station_index) const;

    uint32_t checksum() const;

    static uint32_t magic_value();

private:
    struct Entry {
        uint32_t code_offset;
        uint32_t station_index;
        uint16_t zone;
    };

    std::vector<Entry> entries_;
    std::vector<char> blob_;
    uint32_t checksum_;
};

}

#endif
