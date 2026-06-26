#ifndef IRCT_JOURNEY_PLANNER_H
#define IRCT_JOURNEY_PLANNER_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/fare_table.h"
#include "irct/route_graph.h"

namespace irct {

struct Leg {
    uint32_t from_station;
    uint32_t to_station;
    uint32_t train_number;
    int minutes;
};

struct JourneyPlan {
    std::vector<Leg> legs;
    int total_minutes;
    uint32_t total_fare;
    bool found;
};

class JourneyPlanner {
public:
    JourneyPlanner(const RouteGraph &graph, const FareTable &fares);

    JourneyPlan plan(uint32_t origin, uint32_t destination,
                     FareClass cls) const;

    int estimate_distance_km(int minutes) const;

    size_t max_transfers() const;

    void set_max_transfers(size_t value);

private:
    JourneyPlan reconstruct(uint32_t origin, uint32_t destination,
                            const std::vector<int> &prev,
                            FareClass cls) const;

    const RouteGraph &graph_;
    const FareTable &fares_;
    size_t max_transfers_;
};

}

#endif
