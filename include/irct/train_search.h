#ifndef IRCT_TRAIN_SEARCH_H
#define IRCT_TRAIN_SEARCH_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/train_class.h"

namespace irct {

struct TrainSummary {
    uint32_t number;
    std::string name;
    uint32_t origin;
    uint32_t destination;
    TrainType type;
    uint16_t departure;
    uint16_t arrival;
    uint8_t running_mask;
    uint32_t distance_km;
};

class TrainSearch {
public:
    TrainSearch();

    void add_train(const TrainSummary &summary);

    std::vector<TrainSummary> between(uint32_t origin, uint32_t destination) const;

    std::vector<TrainSummary> by_type(TrainType type) const;

    std::vector<TrainSummary> departing_after(uint32_t origin, uint16_t time) const;

    std::vector<TrainSummary> running_on(uint8_t weekday) const;

    bool fastest_between(uint32_t origin, uint32_t destination,
                         TrainSummary *out) const;

    size_t count() const;

    bool find_by_number(uint32_t number, TrainSummary *out) const;

    std::vector<TrainSummary> sorted_by_departure(uint32_t origin) const;

private:
    int run_minutes(const TrainSummary &summary) const;

    std::vector<TrainSummary> trains_;
};

}

#endif
