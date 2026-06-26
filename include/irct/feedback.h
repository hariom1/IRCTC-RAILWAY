#ifndef IRCT_FEEDBACK_H
#define IRCT_FEEDBACK_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class FeedbackCategory {
    Cleanliness,
    Punctuality,
    Catering,
    Staff,
    Safety,
    Booking
};
struct Feedback {
    std::string pnr;
    FeedbackCategory category;
    uint8_t rating;
    bool resolved;
};
class FeedbackTracker {
public:
    FeedbackTracker();
    void submit(const std::string &pnr, FeedbackCategory category, uint8_t rating);
    double average_rating(FeedbackCategory category) const;
    double overall_rating() const;
    size_t count() const;
    size_t count_category(FeedbackCategory category) const;
    size_t resolve_low_ratings(uint8_t threshold);
    size_t pending_count() const;
    const char *category_label(FeedbackCategory category) const;
    bool worst_category(FeedbackCategory *out) const;
private:
    std::vector<Feedback> entries_;
};
}
#endif
