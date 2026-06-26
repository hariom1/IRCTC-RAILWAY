#ifndef IRCT_RUNNING_STATUS_H
#define IRCT_RUNNING_STATUS_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

enum class StopState {
    Scheduled,
    Arrived,
    Departed,
    Skipped
};

struct StatusPoint {
    uint32_t station;
    uint16_t scheduled_arrival;
    uint16_t actual_arrival;
    StopState state;
};

class RunningStatus {
public:
    explicit RunningStatus(uint32_t train_number);

    void add_point(uint32_t station, uint16_t scheduled_arrival);

    bool mark_arrived(uint32_t station, uint16_t actual_arrival);

    bool mark_departed(uint32_t station);

    bool mark_skipped(uint32_t station);

    int delay_at(uint32_t station) const;

    int average_delay() const;

    uint32_t train_number() const;

    size_t completed_stops() const;

    bool is_running_late() const;

    const char *state_label(StopState state) const;

    bool last_reported(StatusPoint *out) const;

private:
    StatusPoint *find(uint32_t station);

    const StatusPoint *find(uint32_t station) const;

    uint32_t train_number_;
    std::vector<StatusPoint> points_;
};

}

#endif
