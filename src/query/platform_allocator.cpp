#include "irct/platform_allocator.h"

namespace irct {

PlatformAllocator::PlatformAllocator(uint16_t platform_count)
    : platform_count_(platform_count), rejected_(0) {}

bool PlatformAllocator::overlaps(const PlatformSlot &slot, uint16_t arrival,
                                 uint16_t departure) const {
    if (departure <= slot.arrival) {
        return false;
    }
    if (arrival >= slot.departure) {
        return false;
    }
    return true;
}

bool PlatformAllocator::is_free(uint16_t platform, uint16_t arrival,
                                uint16_t departure) const {
    for (size_t i = 0; i < slots_.size(); ++i) {
        if (slots_[i].platform != platform) {
            continue;
        }
        if (overlaps(slots_[i], arrival, departure)) {
            return false;
        }
    }
    return true;
}

bool PlatformAllocator::request(uint32_t train_number, uint16_t arrival,
                                uint16_t departure) {
    if (departure <= arrival) {
        ++rejected_;
        return false;
    }
    for (uint16_t p = 1; p <= platform_count_; ++p) {
        if (is_free(p, arrival, departure)) {
            PlatformSlot slot;
            slot.train_number = train_number;
            slot.arrival = arrival;
            slot.departure = departure;
            slot.platform = p;
            slots_.push_back(slot);
            return true;
        }
    }
    ++rejected_;
    return false;
}

uint16_t PlatformAllocator::platform_of(uint32_t train_number) const {
    for (size_t i = 0; i < slots_.size(); ++i) {
        if (slots_[i].train_number == train_number) {
            return slots_[i].platform;
        }
    }
    return 0;
}

size_t PlatformAllocator::allocated() const {
    return slots_.size();
}

size_t PlatformAllocator::rejected() const {
    return rejected_;
}

std::vector<PlatformSlot> PlatformAllocator::slots_for_platform(
    uint16_t platform) const {
    std::vector<PlatformSlot> out;
    for (size_t i = 0; i < slots_.size(); ++i) {
        if (slots_[i].platform == platform) {
            out.push_back(slots_[i]);
        }
    }
    return out;
}

uint16_t PlatformAllocator::busiest_platform() const {
    if (slots_.empty()) {
        return 0;
    }
    std::vector<uint16_t> counts(platform_count_ + 1, 0);
    for (size_t i = 0; i < slots_.size(); ++i) {
        if (slots_[i].platform <= platform_count_) {
            counts[slots_[i].platform] += 1;
        }
    }
    uint16_t best = 1;
    for (uint16_t p = 2; p <= platform_count_; ++p) {
        if (counts[p] > counts[best]) {
            best = p;
        }
    }
    return best;
}

void PlatformAllocator::reset() {
    slots_.clear();
    rejected_ = 0;
}

}
