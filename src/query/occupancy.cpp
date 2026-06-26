#include "irct/occupancy.h"

namespace irct {

OccupancyTracker::OccupancyTracker(uint16_t capacity) : capacity_(capacity) {}

void OccupancyTracker::add_segment(uint32_t from_station, uint32_t to_station,
                                   uint16_t boarded, uint16_t alighted) {
    SegmentLoad load;
    load.from_station = from_station;
    load.to_station = to_station;
    load.boarded = boarded;
    load.alighted = alighted;
    segments_.push_back(load);
}

std::vector<uint16_t> OccupancyTracker::running_load() const {
    std::vector<uint16_t> loads;
    int current = 0;
    for (size_t i = 0; i < segments_.size(); ++i) {
        current += static_cast<int>(segments_[i].boarded);
        current -= static_cast<int>(segments_[i].alighted);
        if (current < 0) {
            current = 0;
        }
        loads.push_back(static_cast<uint16_t>(current));
    }
    return loads;
}

uint16_t OccupancyTracker::load_at_segment(size_t index) const {
    std::vector<uint16_t> loads = running_load();
    if (index >= loads.size()) {
        return 0;
    }
    return loads[index];
}

uint16_t OccupancyTracker::peak_load() const {
    std::vector<uint16_t> loads = running_load();
    uint16_t peak = 0;
    for (size_t i = 0; i < loads.size(); ++i) {
        if (loads[i] > peak) {
            peak = loads[i];
        }
    }
    return peak;
}

size_t OccupancyTracker::overbooked_segments() const {
    std::vector<uint16_t> loads = running_load();
    size_t count = 0;
    for (size_t i = 0; i < loads.size(); ++i) {
        if (loads[i] > capacity_) {
            ++count;
        }
    }
    return count;
}

double OccupancyTracker::average_occupancy() const {
    std::vector<uint16_t> loads = running_load();
    if (loads.empty() || capacity_ == 0) {
        return 0.0;
    }
    double total = 0.0;
    for (size_t i = 0; i < loads.size(); ++i) {
        total += static_cast<double>(loads[i]);
    }
    double mean = total / static_cast<double>(loads.size());
    return mean / static_cast<double>(capacity_);
}

uint16_t OccupancyTracker::capacity() const {
    return capacity_;
}

size_t OccupancyTracker::segment_count() const {
    return segments_.size();
}

bool OccupancyTracker::busiest_segment(SegmentLoad *out) const {
    std::vector<uint16_t> loads = running_load();
    if (loads.empty()) {
        return false;
    }
    size_t best = 0;
    for (size_t i = 1; i < loads.size(); ++i) {
        if (loads[i] > loads[best]) {
            best = i;
        }
    }
    if (out) {
        *out = segments_[best];
    }
    return true;
}

}
