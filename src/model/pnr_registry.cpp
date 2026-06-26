#include "irct/pnr_registry.h"
namespace irct {
PnrRegistry::PnrRegistry() {}
bool PnrRegistry::valid_pnr(const std::string &pnr) {
    if (pnr.size() != 10) {
        return false;
    }
    for (size_t i = 0; i < pnr.size(); ++i) {
        if (pnr[i] < '0' || pnr[i] > '9') {
            return false;
        }
    }
    return true;
}
bool PnrRegistry::register_ticket(const Ticket &ticket) {
    if (!valid_pnr(ticket.pnr())) {
        return false;
    }
    if (records_.find(ticket.pnr()) != records_.end()) {
        return false;
    }
    Record rec;
    rec.train_number = ticket.train_number();
    rec.passengers = ticket.passenger_count();
    rec.status = ticket.confirmed_count() > 0 ? BookingStatus::Confirmed
                                              : BookingStatus::Waitlisted;
    records_.insert(std::make_pair(ticket.pnr(), rec));
    return true;
}
bool PnrRegistry::exists(const std::string &pnr) const {
    return records_.find(pnr) != records_.end();
}
bool PnrRegistry::status_of(const std::string &pnr, BookingStatus *out) const {
    std::unordered_map<std::string, Record>::const_iterator it = records_.find(pnr);
    if (it == records_.end()) {
        return false;
    }
    if (out) {
        *out = it->second.status;
    }
    return true;
}
size_t PnrRegistry::count() const { return records_.size(); }
std::vector<std::string> PnrRegistry::for_train(uint32_t train_number) const {
    std::vector<std::string> out;
    std::unordered_map<std::string, Record>::const_iterator it;
    for (it = records_.begin(); it != records_.end(); ++it) {
        if (it->second.train_number == train_number) {
            out.push_back(it->first);
        }
    }
    return out;
}
bool PnrRegistry::cancel(const std::string &pnr) {
    std::unordered_map<std::string, Record>::iterator it = records_.find(pnr);
    if (it == records_.end()) {
        return false;
    }
    it->second.status = BookingStatus::Cancelled;
    return true;
}
size_t PnrRegistry::active_count() const {
    size_t total = 0;
    std::unordered_map<std::string, Record>::const_iterator it;
    for (it = records_.begin(); it != records_.end(); ++it) {
        if (it->second.status != BookingStatus::Cancelled) {
            ++total;
        }
    }
    return total;
}
}
