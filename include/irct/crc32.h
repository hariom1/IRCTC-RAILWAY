#ifndef IRCT_CRC32_H
#define IRCT_CRC32_H

#include <cstddef>
#include <cstdint>

namespace irct {

class Crc32 {
public:
    Crc32();

    void reset();

    void update(const uint8_t *data, size_t size);

    uint32_t value() const;

    static uint32_t compute(const uint8_t *data, size_t size);

private:
    uint32_t state_;
    static uint32_t table_[256];
    static bool table_ready_;
    static void build_table();
};

}

#endif
