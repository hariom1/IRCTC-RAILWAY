#include "irct/reservation_queue.h"
namespace irct {
ReservationQueue::ReservationQueue() : next_sequence_(1) {}
uint32_t ReservationQueue::priority_weight(QueuePriority priority) const {
    switch (priority) {
        case QueuePriority::Normal:
            return 1;
        case QueuePriority::Senior:
            return 3;
        case QueuePriority::Defence:
            return 4;
        case QueuePriority::Medical:
            return 5;
        case QueuePriority::Vip:
            return 6;
    }
    return 1;
}
void ReservationQueue::enqueue(const std::string &pnr, QueuePriority priority,
                               uint8_t passengers) {
    QueueTicket ticket;
    ticket.pnr = pnr;
    ticket.priority = priority;
    ticket.passengers = passengers;
    ticket.sequence = next_sequence_;
    ++next_sequence_;
    tickets_.push_back(ticket);
}
size_t ReservationQueue::best_index() const {
    size_t best = 0;
    uint32_t best_weight = priority_weight(tickets_[0].priority);
    uint32_t best_seq = tickets_[0].sequence;
    for (size_t i = 1; i < tickets_.size(); ++i) {
        uint32_t weight = priority_weight(tickets_[i].priority);
        if (weight > best_weight) {
            best = i;
            best_weight = weight;
            best_seq = tickets_[i].sequence;
        } else if (weight == best_weight && tickets_[i].sequence < best_seq) {
            best = i;
            best_seq = tickets_[i].sequence;
        }
    }
    return best;
}
bool ReservationQueue::dequeue(QueueTicket *out) {
    if (tickets_.empty()) {
        return false;
    }
    size_t idx = best_index();
    if (out) {
        *out = tickets_[idx];
    }
    tickets_.erase(tickets_.begin() + static_cast<long>(idx));
    return true;
}
bool ReservationQueue::peek(QueueTicket *out) const {
    if (tickets_.empty()) {
        return false;
    }
    size_t idx = best_index();
    if (out) {
        *out = tickets_[idx];
    }
    return true;
}
size_t ReservationQueue::size() const {
    return tickets_.size();
}
bool ReservationQueue::empty() const {
    return tickets_.empty();
}
size_t ReservationQueue::count_priority(QueuePriority priority) const {
    size_t total = 0;
    for (size_t i = 0; i < tickets_.size(); ++i) {
        if (tickets_[i].priority == priority) {
            ++total;
        }
    }
    return total;
}
uint32_t ReservationQueue::total_passengers() const {
    uint32_t total = 0;
    for (size_t i = 0; i < tickets_.size(); ++i) {
        total += tickets_[i].passengers;
    }
    return total;
}
void ReservationQueue::clear() {
    tickets_.clear();
}
const char *ReservationQueue::priority_label(QueuePriority priority) const {
    switch (priority) {
        case QueuePriority::Normal:
            return "normal";
        case QueuePriority::Senior:
            return "senior";
        case QueuePriority::Defence:
            return "defence";
        case QueuePriority::Medical:
            return "medical";
        case QueuePriority::Vip:
            return "vip";
    }
    return "unknown";
}
}
