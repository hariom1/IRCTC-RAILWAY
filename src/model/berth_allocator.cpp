#include "irct/berth_allocator.h"

namespace irct {

BerthAllocator::BerthAllocator(CoachLayout *layout)
    : layout_(layout), hits_(0) {}

BerthType BerthAllocator::recommend(const BerthRequest &request) const {
    if (request.senior || request.age >= 60) {
        return BerthType::Lower;
    }
    if (request.age < 12) {
        return BerthType::Lower;
    }
    return request.preferred;
}

BerthAssignment BerthAllocator::assign(const BerthRequest &request) {
    BerthAssignment result;
    result.pnr = request.pnr;
    result.berth = 0;
    result.type = request.preferred;
    result.honored_preference = false;

    if (!layout_) {
        return result;
    }

    BerthType wanted = recommend(request);
    uint16_t number = layout_->first_available(wanted);
    if (number == 0) {
        return result;
    }

    layout_->allocate(number);
    result.berth = number;
    result.type = layout_->berth_type(number);
    result.honored_preference = (result.type == wanted);
    if (result.honored_preference) {
        ++hits_;
    }
    assignments_.push_back(result);
    return result;
}

size_t BerthAllocator::assigned_count() const {
    return assignments_.size();
}

size_t BerthAllocator::preference_hits() const {
    return hits_;
}

bool BerthAllocator::release(const std::string &pnr) {
    for (size_t i = 0; i < assignments_.size(); ++i) {
        if (assignments_[i].pnr == pnr) {
            if (layout_) {
                layout_->free(assignments_[i].berth);
            }
            assignments_.erase(assignments_.begin() + static_cast<long>(i));
            return true;
        }
    }
    return false;
}

double BerthAllocator::satisfaction_rate() const {
    if (assignments_.empty()) {
        return 0.0;
    }
    return static_cast<double>(hits_) /
           static_cast<double>(assignments_.size());
}

}
