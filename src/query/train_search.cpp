#include "irct/train_search.h"

#include <algorithm>

#include "irct/schedule.h"

namespace irct {

TrainSearch::TrainSearch() {}

void TrainSearch::add_train(const TrainSummary &summary) {
    trains_.push_back(summary);
}

int TrainSearch::run_minutes(const TrainSummary &summary) const {
    DayTime dep = Schedule::from_packed(summary.departure);
    DayTime arr = Schedule::from_packed(summary.arrival);
    int minutes = static_cast<int>(arr.minutes) - static_cast<int>(dep.minutes);
    if (minutes < 0) {
        minutes += 24 * 60;
    }
    return minutes;
}

std::vector<TrainSummary> TrainSearch::between(uint32_t origin,
                                               uint32_t destination) const {
    std::vector<TrainSummary> out;
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].origin == origin &&
            trains_[i].destination == destination) {
            out.push_back(trains_[i]);
        }
    }
    return out;
}

std::vector<TrainSummary> TrainSearch::by_type(TrainType type) const {
    std::vector<TrainSummary> out;
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].type == type) {
            out.push_back(trains_[i]);
        }
    }
    return out;
}

std::vector<TrainSummary> TrainSearch::departing_after(uint32_t origin,
                                                       uint16_t time) const {
    std::vector<TrainSummary> out;
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].origin == origin && trains_[i].departure >= time) {
            out.push_back(trains_[i]);
        }
    }
    return out;
}

std::vector<TrainSummary> TrainSearch::running_on(uint8_t weekday) const {
    std::vector<TrainSummary> out;
    uint8_t bit = static_cast<uint8_t>(1u << (weekday % 7));
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].running_mask & bit) {
            out.push_back(trains_[i]);
        }
    }
    return out;
}

bool TrainSearch::fastest_between(uint32_t origin, uint32_t destination,
                                  TrainSummary *out) const {
    const TrainSummary *best = nullptr;
    int best_minutes = 0;
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].origin != origin ||
            trains_[i].destination != destination) {
            continue;
        }
        int minutes = run_minutes(trains_[i]);
        if (!best || minutes < best_minutes) {
            best = &trains_[i];
            best_minutes = minutes;
        }
    }
    if (!best) {
        return false;
    }
    if (out) {
        *out = *best;
    }
    return true;
}

size_t TrainSearch::count() const {
    return trains_.size();
}

bool TrainSearch::find_by_number(uint32_t number, TrainSummary *out) const {
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].number == number) {
            if (out) {
                *out = trains_[i];
            }
            return true;
        }
    }
    return false;
}

static bool by_departure(const TrainSummary &a, const TrainSummary &b) {
    return a.departure < b.departure;
}

std::vector<TrainSummary> TrainSearch::sorted_by_departure(
    uint32_t origin) const {
    std::vector<TrainSummary> out;
    for (size_t i = 0; i < trains_.size(); ++i) {
        if (trains_[i].origin == origin) {
            out.push_back(trains_[i]);
        }
    }
    std::sort(out.begin(), out.end(), by_departure);
    return out;
}

}
