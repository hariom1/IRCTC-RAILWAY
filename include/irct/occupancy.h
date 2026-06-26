#ifndef IRCT_OCCUPANCY_H
#define IRCT_OCCUPANCY_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

struct SegmentLoad {
    uint32_t from_station;
    uint32_t to_station;
    uint16_t boarded;
    uint16_t alighted;
};

class OccupancyTracker {
public:
    explicit OccupancyTracker(uint16_t capacity);

    void add_segment(uint32_t from_station, uint32_t to_station,
                     uint16_t boarded, uint16_t alighted);

    uint16_t load_at_segment(size_t index) const;

    uint16_t peak_load() const;

    size_t overbooked_segments() const;

    double average_occupancy() const;

    uint16_t capacity() const;

    size_t segment_count() const;

    bool busiest_segment(SegmentLoad *out) const;

private:
    std::vector<uint16_t> running_load() const;

    uint16_t capacity_;
    std::vector<SegmentLoad> segments_;
};

}

#endif
