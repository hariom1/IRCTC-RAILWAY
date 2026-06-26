#ifndef IRCT_QUOTA_ALLOCATOR_H
#define IRCT_QUOTA_ALLOCATOR_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/availability.h"

namespace irct {

struct QuotaPool {
    Quota quota;
    uint16_t allotted;
    uint16_t booked;
};

class QuotaAllocator {
public:
    QuotaAllocator();

    void configure(Quota quota, uint16_t berths);

    bool book(Quota quota, uint16_t count);

    uint16_t available(Quota quota) const;

    uint16_t total_allotted() const;

    uint16_t total_booked() const;

    bool transfer(Quota from, Quota to, uint16_t count);

    size_t pool_count() const;

    const char *quota_label(Quota quota) const;

    void release(Quota quota, uint16_t count);

private:
    QuotaPool *find(Quota quota);

    const QuotaPool *find(Quota quota) const;

    std::vector<QuotaPool> pools_;
};

}

#endif
