#include "irct/schedule_solver.h"

#include "irct/schedule.h"

namespace irct {

ScheduleSolver::ScheduleSolver() : origin_departure_(0) {}

void ScheduleSolver::set_origin_departure(uint16_t packed_time) {
    origin_departure_ = packed_time;
}

void ScheduleSolver::add_segment(const RouteSegment &segment) {
    segments_.push_back(segment);
}

int ScheduleSolver::travel_minutes(const RouteSegment &segment) const {
    uint16_t speed = segment.average_speed_kmph;
    if (speed == 0) {
        speed = 50;
    }
    int minutes = static_cast<int>((segment.distance_km * 60u) / speed);
    if (minutes < 1) {
        minutes = 1;
    }
    return minutes;
}

std::vector<SolvedStop> ScheduleSolver::solve(uint32_t origin_station) const {
    std::vector<SolvedStop> stops;

    SolvedStop first;
    first.station = origin_station;
    first.arrival = origin_departure_;
    first.departure = origin_departure_;
    first.day_offset = 0;
    first.cumulative_km = 0;
    stops.push_back(first);

    DayTime clock = Schedule::from_packed(origin_departure_);
    int absolute = static_cast<int>(clock.minutes);
    uint32_t cumulative = 0;

    for (size_t i = 0; i < segments_.size(); ++i) {
        const RouteSegment &seg = segments_[i];
        int run = travel_minutes(seg);
        absolute += run;
        cumulative += seg.distance_km;

        int arrival_abs = absolute;
        uint8_t day = static_cast<uint8_t>(arrival_abs / (24 * 60));
        int arrival_in_day = arrival_abs % (24 * 60);

        DayTime arr;
        arr.minutes = static_cast<uint16_t>(arrival_in_day);

        absolute += static_cast<int>(seg.halt_minutes);
        int departure_abs = absolute;
        int departure_in_day = departure_abs % (24 * 60);
        DayTime dep;
        dep.minutes = static_cast<uint16_t>(departure_in_day);

        SolvedStop stop;
        stop.station = seg.to_station;
        stop.arrival = Schedule::to_packed(arr);
        stop.departure = Schedule::to_packed(dep);
        stop.day_offset = day;
        stop.cumulative_km = cumulative;
        stops.push_back(stop);
    }

    return stops;
}

int ScheduleSolver::total_run_minutes() const {
    int total = 0;
    for (size_t i = 0; i < segments_.size(); ++i) {
        total += travel_minutes(segments_[i]);
        total += static_cast<int>(segments_[i].halt_minutes);
    }
    return total;
}

uint32_t ScheduleSolver::total_distance_km() const {
    uint32_t total = 0;
    for (size_t i = 0; i < segments_.size(); ++i) {
        total += segments_[i].distance_km;
    }
    return total;
}

uint16_t ScheduleSolver::average_speed() const {
    int minutes = total_run_minutes();
    if (minutes <= 0) {
        return 0;
    }
    uint32_t km = total_distance_km();
    uint32_t speed = (km * 60u) / static_cast<uint32_t>(minutes);
    return static_cast<uint16_t>(speed);
}

size_t ScheduleSolver::segment_count() const {
    return segments_.size();
}

void ScheduleSolver::clear() {
    segments_.clear();
    origin_departure_ = 0;
}

}
