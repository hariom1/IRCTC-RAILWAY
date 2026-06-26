#ifndef IRCT_PLATFORM_ALLOCATOR_H
#define IRCT_PLATFORM_ALLOCATOR_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

struct PlatformSlot {
    uint32_t train_number;
    uint16_t arrival;
    uint16_t departure;
    uint16_t platform;
};

class PlatformAllocator {
public:
    explicit PlatformAllocator(uint16_t platform_count);

    bool request(uint32_t train_number, uint16_t arrival, uint16_t departure);

    uint16_t platform_of(uint32_t train_number) const;

    size_t allocated() const;

    size_t rejected() const;

    bool is_free(uint16_t platform, uint16_t arrival, uint16_t departure) const;

    std::vector<PlatformSlot> slots_for_platform(uint16_t platform) const;

    uint16_t busiest_platform() const;

    void reset();

private:
    bool overlaps(const PlatformSlot &slot, uint16_t arrival,
                  uint16_t departure) const;

    uint16_t platform_count_;
    std::vector<PlatformSlot> slots_;
    size_t rejected_;
};

}

#endif
