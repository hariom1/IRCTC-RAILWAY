#ifndef IRCT_BYTEBUFFER_H
#define IRCT_BYTEBUFFER_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace irct {

class ByteBuffer {
public:
    ByteBuffer();

    void put_u8(uint8_t v);

    void put_u16(uint16_t v);

    void put_u32(uint32_t v);

    void put_bytes(const uint8_t *data, size_t size);

    void put_cstr(const char *s);

    size_t size() const;

    const uint8_t *data() const;

    void patch_u32(size_t offset, uint32_t v);

    void clear();

    std::vector<uint8_t> take();

private:
    std::vector<uint8_t> bytes_;
};

}

#endif
