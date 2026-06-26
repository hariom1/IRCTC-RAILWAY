#include "irct/feedback.h"
namespace irct {
FeedbackTracker::FeedbackTracker() {}
void FeedbackTracker::submit(const std::string &pnr,
                             FeedbackCategory category, uint8_t rating) {
    Feedback entry;
    entry.pnr = pnr;
    entry.category = category;
    entry.rating = rating > 5 ? 5 : rating;
    entry.resolved = false;
    entries_.push_back(entry);
}
double FeedbackTracker::average_rating(FeedbackCategory category) const {
    uint32_t total = 0;
    uint32_t counted = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].category == category) {
            total += entries_[i].rating;
            ++counted;
        }
    }
    if (counted == 0) {
        return 0.0;
    }
    return static_cast<double>(total) / static_cast<double>(counted);
}
double FeedbackTracker::overall_rating() const {
    if (entries_.empty()) {
        return 0.0;
    }
    uint32_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        total += entries_[i].rating;
    }
    return static_cast<double>(total) / static_cast<double>(entries_.size());
}
size_t FeedbackTracker::count() const {
    return entries_.size();
}
size_t FeedbackTracker::count_category(FeedbackCategory category) const {
    size_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].category == category) {
            ++total;
        }
    }
    return total;
}
size_t FeedbackTracker::resolve_low_ratings(uint8_t threshold) {
    size_t resolved = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (!entries_[i].resolved && entries_[i].rating < threshold) {
            entries_[i].resolved = true;
            ++resolved;
        }
    }
    return resolved;
}
size_t FeedbackTracker::pending_count() const {
    size_t total = 0;
    for (size_t i = 0; i < entries_.size(); ++i) {
        if (!entries_[i].resolved) {
            ++total;
        }
    }
    return total;
}
bool FeedbackTracker::worst_category(FeedbackCategory *out) const {
    if (entries_.empty()) {
        return false;
    }
    FeedbackCategory categories[6] = {
        FeedbackCategory::Cleanliness, FeedbackCategory::Punctuality,
        FeedbackCategory::Catering,    FeedbackCategory::Staff,
        FeedbackCategory::Safety,      FeedbackCategory::Booking};
    FeedbackCategory worst = categories[0];
    double worst_avg = -1.0;
    for (int i = 0; i < 6; ++i) {
        if (count_category(categories[i]) == 0) {
            continue;
        }
        double avg = average_rating(categories[i]);
        if (worst_avg < 0.0 || avg < worst_avg) {
            worst_avg = avg;
            worst = categories[i];
        }
    }
    if (worst_avg < 0.0) {
        return false;
    }
    if (out) {
        *out = worst;
    }
    return true;
}
const char *FeedbackTracker::category_label(FeedbackCategory category) const {
    switch (category) {
        case FeedbackCategory::Cleanliness:
            return "cleanliness";
        case FeedbackCategory::Punctuality:
            return "punctuality";
        case FeedbackCategory::Catering:
            return "catering";
        case FeedbackCategory::Staff:
            return "staff";
        case FeedbackCategory::Safety:
            return "safety";
        case FeedbackCategory::Booking:
            return "booking";
    }
    return "unknown";
}
}
