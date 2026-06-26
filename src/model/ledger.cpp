#include "irct/ledger.h"

namespace irct {

Ledger::Ledger() : next_sequence_(1) {}

void Ledger::record(const std::string &pnr, EntryKind kind,
                    int64_t amount_paise) {
    LedgerEntry entry;
    entry.pnr = pnr;
    entry.kind = kind;
    entry.amount_paise = amount_paise;
    entry.sequence = next_sequence_;
    ++next_sequence_;
    entries_.push_back(entry);
}

int64_t Ledger::balance() const {
    int64_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        total += entries_[i].amount_paise;
    }
    return total;
}

int64_t Ledger::total_for(EntryKind kind) const {
    int64_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].kind == kind) {
            total += entries_[i].amount_paise;
        }
    }
    return total;
}

int64_t Ledger::net_for_pnr(const std::string &pnr) const {
    int64_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].pnr == pnr) {
            total += entries_[i].amount_paise;
        }
    }
    return total;
}

size_t Ledger::entry_count() const {
    return entries_.size();
}

std::vector<LedgerEntry> Ledger::entries_for(const std::string &pnr) const {
    std::vector<LedgerEntry> out;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].pnr == pnr) {
            out.push_back(entries_[i]);
        }
    }
    return out;
}

const char *Ledger::kind_label(EntryKind kind) const {
    switch (kind) {
        case EntryKind::Booking:
            return "booking";
        case EntryKind::Refund:
            return "refund";
        case EntryKind::Cancellation:
            return "cancellation";
        case EntryKind::Surcharge:
            return "surcharge";
        case EntryKind::Concession:
            return "concession";
    }
    return "unknown";
}

bool Ledger::is_balanced_for(const std::string &pnr) const {
    return net_for_pnr(pnr) == 0;
}

}
