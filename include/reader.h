#ifndef IRCT_READER_H
#define IRCT_READER_H

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace irct {

class Reader {
public:
    Reader(const uint8_t *data, size_t size)
        : data_(data), size_(size), pos_(0), ok_(true) {}

    bool ok() const { return ok_; }

    size_t pos() const { return pos_; }

    size_t remaining() const { return pos_ <= size_ ? size_ - pos_ : 0; }

    void seek(size_t p) {
        if (p > size_) {
            ok_ = false;
            return;
        }
        pos_ = p;
    }

    uint8_t u8() {
        if (remaining() < 1) {
            ok_ = false;
            return 0;
        }
        return data_[pos_++];
    }

    uint16_t u16() {
        if (remaining() < 2) {
            ok_ = false;
            return 0;
        }
        uint16_t v = static_cast<uint16_t>(data_[pos_]) |
                     (static_cast<uint16_t>(data_[pos_ + 1]) << 8);
        pos_ += 2;
        return v;
    }

    uint32_t u32() {
        if (remaining() < 4) {
            ok_ = false;
            return 0;
        }
        uint32_t v = static_cast<uint32_t>(data_[pos_]) |
                     (static_cast<uint32_t>(data_[pos_ + 1]) << 8) |
                     (static_cast<uint32_t>(data_[pos_ + 2]) << 16) |
                     (static_cast<uint32_t>(data_[pos_ + 3]) << 24);
        pos_ += 4;
        return v;
    }

    const uint8_t *take(size_t n) {
        if (remaining() < n) {
            ok_ = false;
            return nullptr;
        }
        const uint8_t *p = data_ + pos_;
        pos_ += n;
        return p;
    }

private:
    const uint8_t *data_;
    size_t size_;
    size_t pos_;
    bool ok_;
};

}

#endif
