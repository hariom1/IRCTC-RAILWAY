#ifndef IRCT_SCHEDULE_SOLVER_H
#define IRCT_SCHEDULE_SOLVER_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

struct RouteSegment {
    uint32_t from_station;
    uint32_t to_station;
    uint32_t distance_km;
    uint16_t average_speed_kmph;
    uint16_t halt_minutes;
};

struct SolvedStop {
    uint32_t station;
    uint16_t arrival;
    uint16_t departure;
    uint8_t day_offset;
    uint32_t cumulative_km;
};

class ScheduleSolver {
public:
    ScheduleSolver();

    void set_origin_departure(uint16_t packed_time);

    void add_segment(const RouteSegment &segment);

    std::vector<SolvedStop> solve(uint32_t origin_station) const;

    int total_run_minutes() const;

    uint32_t total_distance_km() const;

    uint16_t average_speed() const;

    size_t segment_count() const;

    void clear();

private:
    int travel_minutes(const RouteSegment &segment) const;

    uint16_t origin_departure_;
    std::vector<RouteSegment> segments_;
};

}

#endif
