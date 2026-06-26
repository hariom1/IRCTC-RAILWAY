#ifndef IRCT_RESERVATION_QUEUE_H
#define IRCT_RESERVATION_QUEUE_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class QueuePriority {
    Normal,
    Senior,
    Defence,
    Medical,
    Vip
};
struct QueueTicket {
    std::string pnr;
    QueuePriority priority;
    uint32_t sequence;
    uint8_t passengers;
};
class ReservationQueue {
public:
    ReservationQueue();
    void enqueue(const std::string &pnr, QueuePriority priority, uint8_t passengers);
    bool dequeue(QueueTicket *out);
    size_t size() const;
    bool empty() const;
    size_t count_priority(QueuePriority priority) const;
    uint32_t total_passengers() const;
    bool peek(QueueTicket *out) const;
    void clear();
    const char *priority_label(QueuePriority priority) const;
    uint32_t priority_weight(QueuePriority priority) const;
private:
    size_t best_index() const;
    std::vector<QueueTicket> tickets_;
    uint32_t next_sequence_;
};
}
#endif
