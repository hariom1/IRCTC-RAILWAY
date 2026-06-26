#ifndef IRCT_FARE_SPLIT_H
#define IRCT_FARE_SPLIT_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace irct {

struct SegmentShare {
    uint32_t from_station;
    uint32_t to_station;
    uint32_t distance_km;
    uint32_t share_paise;
};

class FareSplitter {
public:
    FareSplitter();

    void add_segment(uint32_t from_station, uint32_t to_station,
                     uint32_t distance_km);

    std::vector<SegmentShare> split(uint32_t total_paise) const;

    uint32_t total_distance() const;

    size_t segment_count() const;

    uint32_t largest_share(uint32_t total_paise) const;

    bool balanced(uint32_t total_paise, uint32_t tolerance) const;

    void clear();

private:
    std::vector<SegmentShare> segments_;
};

}

#endif
