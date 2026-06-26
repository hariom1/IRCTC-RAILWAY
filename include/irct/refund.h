#ifndef IRCT_REFUND_H
#define IRCT_REFUND_H

#include <cstdint>
#include <string>

#include "irct/fare_table.h"
#include "irct/ticket.h"

namespace irct {

enum class CancelWindow {
    MoreThan48h,
    Between12And48h,
    Between4And12h,
    LessThan4h,
    AfterDeparture
};

struct RefundBreakdown {
    uint32_t fare_paid;
    uint32_t cancellation_fee;
    uint32_t clerkage;
    uint32_t refund_amount;
    bool refundable;
};

class RefundCalculator {
public:
    RefundCalculator();

    RefundBreakdown compute(uint32_t fare_paid, FareClass cls,
                            CancelWindow window, BookingStatus status) const;

    uint32_t flat_fee(FareClass cls) const;

    uint32_t clerkage_fee(BookingStatus status) const;

    CancelWindow window_from_hours(int hours_before) const;

    const char *window_label(CancelWindow window) const;

private:
    uint32_t percentage_fee(uint32_t fare, CancelWindow window) const;
};

}

#endif
