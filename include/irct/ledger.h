#ifndef IRCT_LEDGER_H
#define IRCT_LEDGER_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

enum class EntryKind {
    Booking,
    Refund,
    Cancellation,
    Surcharge,
    Concession
};

struct LedgerEntry {
    std::string pnr;
    EntryKind kind;
    int64_t amount_paise;
    uint32_t sequence;
};

class Ledger {
public:
    Ledger();

    void record(const std::string &pnr, EntryKind kind, int64_t amount_paise);

    int64_t balance() const;

    int64_t total_for(EntryKind kind) const;

    int64_t net_for_pnr(const std::string &pnr) const;

    size_t entry_count() const;

    std::vector<LedgerEntry> entries_for(const std::string &pnr) const;

    const char *kind_label(EntryKind kind) const;

    bool is_balanced_for(const std::string &pnr) const;

private:
    std::vector<LedgerEntry> entries_;
    uint32_t next_sequence_;
};

}

#endif
