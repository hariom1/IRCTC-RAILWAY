#ifndef IRCT_PNR_REGISTRY_H
#define IRCT_PNR_REGISTRY_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "irct/ticket.h"
namespace irct {
class PnrRegistry {
public:
    PnrRegistry();
    bool register_ticket(const Ticket &ticket);
    bool exists(const std::string &pnr) const;
    bool status_of(const std::string &pnr, BookingStatus *out) const;
    size_t count() const;
    std::vector<std::string> for_train(uint32_t train_number) const;
    bool cancel(const std::string &pnr);
    size_t active_count() const;
    static bool valid_pnr(const std::string &pnr);
private:
    struct Record { uint32_t train_number; BookingStatus status; size_t passengers; };
    std::unordered_map<std::string, Record> records_;
};
}
#endif
