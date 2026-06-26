#include "irct/refund.h"

namespace irct {

RefundCalculator::RefundCalculator() {}

uint32_t RefundCalculator::flat_fee(FareClass cls) const {
    switch (cls) {
        case FareClass::Second:
            return 3000;
        case FareClass::Sleeper:
            return 6000;
        case FareClass::ThirdAc:
            return 12000;
        case FareClass::SecondAc:
            return 16000;
        case FareClass::FirstAc:
            return 24000;
    }
    return 6000;
}

uint32_t RefundCalculator::clerkage_fee(BookingStatus status) const {
    if (status == BookingStatus::Waitlisted ||
        status == BookingStatus::RacWaiting) {
        return 6000;
    }
    return 0;
}

CancelWindow RefundCalculator::window_from_hours(int hours_before) const {
    if (hours_before < 0) {
        return CancelWindow::AfterDeparture;
    }
    if (hours_before >= 48) {
        return CancelWindow::MoreThan48h;
    }
    if (hours_before >= 12) {
        return CancelWindow::Between12And48h;
    }
    if (hours_before >= 4) {
        return CancelWindow::Between4And12h;
    }
    return CancelWindow::LessThan4h;
}

const char *RefundCalculator::window_label(CancelWindow window) const {
    switch (window) {
        case CancelWindow::MoreThan48h:
            return "more than 48 hours";
        case CancelWindow::Between12And48h:
            return "12 to 48 hours";
        case CancelWindow::Between4And12h:
            return "4 to 12 hours";
        case CancelWindow::LessThan4h:
            return "less than 4 hours";
        case CancelWindow::AfterDeparture:
            return "after departure";
    }
    return "unknown";
}

uint32_t RefundCalculator::percentage_fee(uint32_t fare,
                                          CancelWindow window) const {
    switch (window) {
        case CancelWindow::MoreThan48h:
            return 0;
        case CancelWindow::Between12And48h:
            return (fare * 25u) / 100u;
        case CancelWindow::Between4And12h:
            return (fare * 50u) / 100u;
        case CancelWindow::LessThan4h:
            return fare;
        case CancelWindow::AfterDeparture:
            return fare;
    }
    return fare;
}

RefundBreakdown RefundCalculator::compute(uint32_t fare_paid, FareClass cls,
                                          CancelWindow window,
                                          BookingStatus status) const {
    RefundBreakdown out;
    out.fare_paid = fare_paid;
    out.clerkage = clerkage_fee(status);
    out.refundable = true;

    if (status == BookingStatus::Cancelled) {
        out.cancellation_fee = 0;
        out.refund_amount = 0;
        out.refundable = false;
        return out;
    }

    if (window == CancelWindow::AfterDeparture ||
        window == CancelWindow::LessThan4h) {
        out.cancellation_fee = fare_paid;
        out.refund_amount = 0;
        out.refundable = false;
        return out;
    }

    uint32_t flat = flat_fee(cls);
    uint32_t pct = percentage_fee(fare_paid, window);
    uint32_t fee = pct > flat ? pct : flat;
    out.cancellation_fee = fee;

    uint32_t deductions = fee + out.clerkage;
    if (deductions >= fare_paid) {
        out.refund_amount = 0;
    } else {
        out.refund_amount = fare_paid - deductions;
    }
    return out;
}

}
