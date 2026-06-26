#include "irct/running_status.h"

#include "irct/schedule.h"

namespace irct {

RunningStatus::RunningStatus(uint32_t train_number)
    : train_number_(train_number) {}

StatusPoint *RunningStatus::find(uint32_t station) {
    for (size_t i = 0; i < points_.size(); ++i) {
        if (points_[i].station == station) {
            return &points_[i];
        }
    }
    return nullptr;
}

const StatusPoint *RunningStatus::find(uint32_t station) const {
    for (size_t i = 0; i < points_.size(); ++i) {
        if (points_[i].station == station) {
            return &points_[i];
        }
    }
    return nullptr;
}

void RunningStatus::add_point(uint32_t station, uint16_t scheduled_arrival) {
    StatusPoint point;
    point.station = station;
    point.scheduled_arrival = scheduled_arrival;
    point.actual_arrival = 0;
    point.state = StopState::Scheduled;
    points_.push_back(point);
}

bool RunningStatus::mark_arrived(uint32_t station, uint16_t actual_arrival) {
    StatusPoint *point = find(station);
    if (!point) {
        return false;
    }
    point->actual_arrival = actual_arrival;
    point->state = StopState::Arrived;
    return true;
}

bool RunningStatus::mark_departed(uint32_t station) {
    StatusPoint *point = find(station);
    if (!point) {
        return false;
    }
    point->state = StopState::Departed;
    return true;
}

bool RunningStatus::mark_skipped(uint32_t station) {
    StatusPoint *point = find(station);
    if (!point) {
        return false;
    }
    point->state = StopState::Skipped;
    return true;
}

int RunningStatus::delay_at(uint32_t station) const {
    const StatusPoint *point = find(station);
    if (!point) {
        return 0;
    }
    if (point->state == StopState::Scheduled ||
        point->state == StopState::Skipped) {
        return 0;
    }
    DayTime scheduled = Schedule::from_packed(point->scheduled_arrival);
    DayTime actual = Schedule::from_packed(point->actual_arrival);
    return static_cast<int>(actual.minutes) -
           static_cast<int>(scheduled.minutes);
}

int RunningStatus::average_delay() const {
    int total = 0;
    int counted = 0;
    for (size_t i = 0; i < points_.size(); ++i) {
        if (points_[i].state == StopState::Arrived ||
            points_[i].state == StopState::Departed) {
            total += delay_at(points_[i].station);
            ++counted;
        }
    }
    if (counted == 0) {
        return 0;
    }
    return total / counted;
}

uint32_t RunningStatus::train_number() const {
    return train_number_;
}

size_t RunningStatus::completed_stops() const {
    size_t total = 0;
    for (size_t i = 0; i < points_.size(); ++i) {
        if (points_[i].state == StopState::Departed) {
            ++total;
        }
    }
    return total;
}

bool RunningStatus::is_running_late() const {
    return average_delay() > 15;
}

const char *RunningStatus::state_label(StopState state) const {
    switch (state) {
        case StopState::Scheduled:
            return "scheduled";
        case StopState::Arrived:
            return "arrived";
        case StopState::Departed:
            return "departed";
        case StopState::Skipped:
            return "skipped";
    }
    return "unknown";
}

bool RunningStatus::last_reported(StatusPoint *out) const {
    const StatusPoint *last = nullptr;
    for (size_t i = 0; i < points_.size(); ++i) {
        if (points_[i].state == StopState::Arrived ||
            points_[i].state == StopState::Departed) {
            last = &points_[i];
        }
    }
    if (!last) {
        return false;
    }
    if (out) {
        *out = *last;
    }
    return true;
}

}
