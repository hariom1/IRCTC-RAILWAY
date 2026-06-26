#include "irct/quota_allocator.h"

namespace irct {

QuotaAllocator::QuotaAllocator() {}

QuotaPool *QuotaAllocator::find(Quota quota) {
    for (size_t i = 0; i < pools_.size(); ++i) {
        if (pools_[i].quota == quota) {
            return &pools_[i];
        }
    }
    return nullptr;
}

const QuotaPool *QuotaAllocator::find(Quota quota) const {
    for (size_t i = 0; i < pools_.size(); ++i) {
        if (pools_[i].quota == quota) {
            return &pools_[i];
        }
    }
    return nullptr;
}

void QuotaAllocator::configure(Quota quota, uint16_t berths) {
    QuotaPool *existing = find(quota);
    if (existing) {
        existing->allotted = berths;
        return;
    }
    QuotaPool pool;
    pool.quota = quota;
    pool.allotted = berths;
    pool.booked = 0;
    pools_.push_back(pool);
}

bool QuotaAllocator::book(Quota quota, uint16_t count) {
    QuotaPool *pool = find(quota);
    if (!pool) {
        return false;
    }
    uint16_t free_here = 0;
    if (pool->booked < pool->allotted) {
        free_here = static_cast<uint16_t>(pool->allotted - pool->booked);
    }
    if (free_here < count) {
        return false;
    }
    pool->booked = static_cast<uint16_t>(pool->booked + count);
    return true;
}

uint16_t QuotaAllocator::available(Quota quota) const {
    const QuotaPool *pool = find(quota);
    if (!pool) {
        return 0;
    }
    if (pool->booked >= pool->allotted) {
        return 0;
    }
    return static_cast<uint16_t>(pool->allotted - pool->booked);
}

uint16_t QuotaAllocator::total_allotted() const {
    uint32_t total = 0;
    for (size_t i = 0; i < pools_.size(); ++i) {
        total += pools_[i].allotted;
    }
    if (total > 0xFFFFu) {
        return 0xFFFFu;
    }
    return static_cast<uint16_t>(total);
}

uint16_t QuotaAllocator::total_booked() const {
    uint32_t total = 0;
    for (size_t i = 0; i < pools_.size(); ++i) {
        total += pools_[i].booked;
    }
    if (total > 0xFFFFu) {
        return 0xFFFFu;
    }
    return static_cast<uint16_t>(total);
}

bool QuotaAllocator::transfer(Quota from, Quota to, uint16_t count) {
    QuotaPool *source = find(from);
    QuotaPool *target = find(to);
    if (!source || !target) {
        return false;
    }
    uint16_t free_here = 0;
    if (source->booked < source->allotted) {
        free_here = static_cast<uint16_t>(source->allotted - source->booked);
    }
    if (free_here < count) {
        return false;
    }
    source->allotted = static_cast<uint16_t>(source->allotted - count);
    target->allotted = static_cast<uint16_t>(target->allotted + count);
    return true;
}

size_t QuotaAllocator::pool_count() const {
    return pools_.size();
}

void QuotaAllocator::release(Quota quota, uint16_t count) {
    QuotaPool *pool = find(quota);
    if (!pool) {
        return;
    }
    if (pool->booked < count) {
        pool->booked = 0;
    } else {
        pool->booked = static_cast<uint16_t>(pool->booked - count);
    }
}

const char *QuotaAllocator::quota_label(Quota quota) const {
    switch (quota) {
        case Quota::General:
            return "general";
        case Quota::Tatkal:
            return "tatkal";
        case Quota::Ladies:
            return "ladies";
        case Quota::SeniorCitizen:
            return "senior citizen";
        case Quota::Premium:
            return "premium tatkal";
    }
    return "unknown";
}

}
