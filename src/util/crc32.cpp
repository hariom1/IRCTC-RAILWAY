#include "irct/crc32.h"

namespace irct {

uint32_t Crc32::table_[256];
bool Crc32::table_ready_ = false;

void Crc32::build_table() {
    const uint32_t poly = 0xEDB88320u;
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t c = i;
        for (int k = 0; k < 8; ++k) {
            if (c & 1u) {
                c = poly ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        table_[i] = c;
    }
    table_ready_ = true;
}

Crc32::Crc32() : state_(0xFFFFFFFFu) {
    if (!table_ready_) {
        build_table();
    }
}

void Crc32::reset() {
    state_ = 0xFFFFFFFFu;
}

void Crc32::update(const uint8_t *data, size_t size) {
    uint32_t c = state_;
    for (size_t i = 0; i < size; ++i) {
        c = table_[(c ^ data[i]) & 0xFFu] ^ (c >> 8);
    }
    state_ = c;
}

uint32_t Crc32::value() const {
    return state_ ^ 0xFFFFFFFFu;
}

uint32_t Crc32::compute(const uint8_t *data, size_t size) {
    Crc32 c;
    c.update(data, size);
    return c.value();
}

}
