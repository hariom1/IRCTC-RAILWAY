#include "irct/fare_split.h"

namespace irct {

FareSplitter::FareSplitter() {}

void FareSplitter::add_segment(uint32_t from_station, uint32_t to_station,
                               uint32_t distance_km) {
    SegmentShare seg;
    seg.from_station = from_station;
    seg.to_station = to_station;
    seg.distance_km = distance_km;
    seg.share_paise = 0;
    segments_.push_back(seg);
}

uint32_t FareSplitter::total_distance() const {
    uint32_t total = 0;
    for (size_t i = 0; i < segments_.size(); ++i) {
        total += segments_[i].distance_km;
    }
    return total;
}

std::vector<SegmentShare> FareSplitter::split(uint32_t total_paise) const {
    std::vector<SegmentShare> result = segments_;
    uint32_t distance = total_distance();
    if (distance == 0 || result.empty()) {
        return result;
    }

    uint32_t assigned = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        uint64_t portion = static_cast<uint64_t>(total_paise) *
                           result[i].distance_km / distance;
        result[i].share_paise = static_cast<uint32_t>(portion);
        assigned += result[i].share_paise;
    }

    uint32_t remainder = total_paise > assigned ? total_paise - assigned : 0;
    size_t idx = 0;
    while (remainder > 0 && !result.empty()) {
        result[idx].share_paise += 1;
        --remainder;
        idx = (idx + 1) % result.size();
    }

    return result;
}

size_t FareSplitter::segment_count() const {
    return segments_.size();
}

uint32_t FareSplitter::largest_share(uint32_t total_paise) const {
    std::vector<SegmentShare> shares = split(total_paise);
    uint32_t largest = 0;
    for (size_t i = 0; i < shares.size(); ++i) {
        if (shares[i].share_paise > largest) {
            largest = shares[i].share_paise;
        }
    }
    return largest;
}

bool FareSplitter::balanced(uint32_t total_paise, uint32_t tolerance) const {
    std::vector<SegmentShare> shares = split(total_paise);
    if (shares.empty()) {
        return true;
    }
    uint32_t lo = shares[0].share_paise;
    uint32_t hi = shares[0].share_paise;
    for (size_t i = 1; i < shares.size(); ++i) {
        if (shares[i].share_paise < lo) {
            lo = shares[i].share_paise;
        }
        if (shares[i].share_paise > hi) {
            hi = shares[i].share_paise;
        }
    }
    return (hi - lo) <= tolerance;
}

void FareSplitter::clear() {
    segments_.clear();
}

}
