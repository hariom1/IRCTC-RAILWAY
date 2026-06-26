#include "irct/bytebuffer.h"

#include <cstring>

namespace irct {

ByteBuffer::ByteBuffer() {}

void ByteBuffer::put_u8(uint8_t v) {
    bytes_.push_back(v);
}

void ByteBuffer::put_u16(uint16_t v) {
    bytes_.push_back(static_cast<uint8_t>(v & 0xFFu));
    bytes_.push_back(static_cast<uint8_t>((v >> 8) & 0xFFu));
}

void ByteBuffer::put_u32(uint32_t v) {
    bytes_.push_back(static_cast<uint8_t>(v & 0xFFu));
    bytes_.push_back(static_cast<uint8_t>((v >> 8) & 0xFFu));
    bytes_.push_back(static_cast<uint8_t>((v >> 16) & 0xFFu));
    bytes_.push_back(static_cast<uint8_t>((v >> 24) & 0xFFu));
}

void ByteBuffer::put_bytes(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        bytes_.push_back(data[i]);
    }
}

void ByteBuffer::put_cstr(const char *s) {
    size_t n = std::strlen(s);
    for (size_t i = 0; i < n; ++i) {
        bytes_.push_back(static_cast<uint8_t>(s[i]));
    }
    bytes_.push_back(0);
}

size_t ByteBuffer::size() const {
    return bytes_.size();
}

const uint8_t *ByteBuffer::data() const {
    return bytes_.data();
}

void ByteBuffer::patch_u32(size_t offset, uint32_t v) {
    if (offset + 4 > bytes_.size()) {
        return;
    }
    bytes_[offset] = static_cast<uint8_t>(v & 0xFFu);
    bytes_[offset + 1] = static_cast<uint8_t>((v >> 8) & 0xFFu);
    bytes_[offset + 2] = static_cast<uint8_t>((v >> 16) & 0xFFu);
    bytes_[offset + 3] = static_cast<uint8_t>((v >> 24) & 0xFFu);
}

void ByteBuffer::clear() {
    bytes_.clear();
}

std::vector<uint8_t> ByteBuffer::take() {
    std::vector<uint8_t> out;
    out.swap(bytes_);
    return out;
}

}
