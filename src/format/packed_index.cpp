#include "irct/packed_index.h"

#include <cstring>

#include "irct/bytebuffer.h"
#include "irct/crc32.h"
#include "irct/reader_compat.h"

namespace irct {

static const uint32_t kPackedMagic = 0x58444950u;

PackedIndex::PackedIndex() : checksum_(0) {}

uint32_t PackedIndex::magic_value() {
    return kPackedMagic;
}

std::vector<uint8_t> PackedIndex::serialize(const StationIndex &index) const {
    ByteBuffer blob;
    std::vector<uint32_t> offsets;
    std::vector<uint32_t> station_indices;
    std::vector<uint16_t> zones;

    for (size_t i = 0; i < index.size(); ++i) {
        const StationRecord &rec = index.at(i);
        offsets.push_back(static_cast<uint32_t>(blob.size()));
        blob.put_cstr(rec.code.c_str());
        station_indices.push_back(rec.index);
        zones.push_back(rec.zone);
    }

    ByteBuffer body;
    for (size_t i = 0; i < offsets.size(); ++i) {
        body.put_u32(offsets[i]);
        body.put_u32(station_indices[i]);
        body.put_u16(zones[i]);
    }
    body.put_bytes(blob.data(), blob.size());

    uint32_t crc = Crc32::compute(body.data(), body.size());

    ByteBuffer out;
    out.put_u32(kPackedMagic);
    out.put_u32(static_cast<uint32_t>(offsets.size()));
    out.put_u32(static_cast<uint32_t>(blob.size()));
    out.put_u32(crc);
    out.put_bytes(body.data(), body.size());
    return out.take();
}

bool PackedIndex::deserialize(const uint8_t *data, size_t size) {
    entries_.clear();
    blob_.clear();

    ReaderCompat r(data, size);
    uint32_t magic = r.u32();
    if (!r.ok() || magic != kPackedMagic) {
        return false;
    }
    uint32_t entry_count = r.u32();
    uint32_t blob_len = r.u32();
    uint32_t crc = r.u32();
    if (!r.ok()) {
        return false;
    }
    checksum_ = crc;

    if (entry_count > (1u << 20) || blob_len > (1u << 22)) {
        return false;
    }

    for (uint32_t i = 0; i < entry_count; ++i) {
        Entry e;
        e.code_offset = r.u32();
        e.station_index = r.u32();
        e.zone = r.u16();
        if (!r.ok()) {
            return false;
        }
        if (e.code_offset >= blob_len) {
            return false;
        }
        entries_.push_back(e);
    }

    const uint8_t *raw = r.take(blob_len);
    if (!raw) {
        return false;
    }
    blob_.resize(blob_len + 1);
    std::memcpy(blob_.data(), raw, blob_len);
    blob_[blob_len] = '\0';
    return true;
}

size_t PackedIndex::entry_count() const {
    return entries_.size();
}

bool PackedIndex::find(const std::string &code,
                       uint32_t *station_index) const {
    for (size_t i = 0; i < entries_.size(); ++i) {
        const Entry &e = entries_[i];
        if (e.code_offset >= blob_.size()) {
            continue;
        }
        const char *s = blob_.data() + e.code_offset;
        if (code == s) {
            if (station_index) {
                *station_index = e.station_index;
            }
            return true;
        }
    }
    return false;
}

uint32_t PackedIndex::checksum() const {
    return checksum_;
}

}
