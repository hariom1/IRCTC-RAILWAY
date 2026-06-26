#ifndef IRCT_BERTH_ALLOCATOR_H
#define IRCT_BERTH_ALLOCATOR_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/coach_layout.h"

namespace irct {

struct BerthRequest {
    std::string pnr;
    BerthType preferred;
    uint8_t age;
    bool senior;
};

struct BerthAssignment {
    std::string pnr;
    uint16_t berth;
    BerthType type;
    bool honored_preference;
};

class BerthAllocator {
public:
    explicit BerthAllocator(CoachLayout *layout);

    BerthAssignment assign(const BerthRequest &request);

    size_t assigned_count() const;

    size_t preference_hits() const;

    BerthType recommend(const BerthRequest &request) const;

    bool release(const std::string &pnr);

    double satisfaction_rate() const;

private:
    CoachLayout *layout_;
    std::vector<BerthAssignment> assignments_;
    size_t hits_;
};

}

#endif
