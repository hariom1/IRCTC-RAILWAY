#ifndef IRCT_WAITLIST_H
#define IRCT_WAITLIST_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
struct WaitlistItem {
    std::string pnr;
    uint32_t position;
    uint8_t passengers;
};
class Waitlist {
public:
    Waitlist();
    void enqueue(const std::string &pnr, uint8_t passengers);
    bool promote(uint32_t freed_berths);
    uint32_t position_of(const std::string &pnr) const;
    size_t pending() const;
    size_t confirmed() const;
    bool remove(const std::string &pnr);
    void renumber();
    uint32_t total_waiting_passengers() const;
private:
    std::vector<WaitlistItem> items_;
    std::vector<std::string> confirmed_;
};
}
#endif
