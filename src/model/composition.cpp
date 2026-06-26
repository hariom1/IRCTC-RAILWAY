#include "irct/composition.h"

namespace irct {

TrainComposition::TrainComposition() : next_position_(1) {
    empty_.code = "";
    empty_.cls = FareClass::Sleeper;
    empty_.position = 0;
    empty_.capacity = 0;
}

void TrainComposition::append(const std::string &code, FareClass cls,
                              uint16_t capacity) {
    CoachUnit unit;
    unit.code = code;
    unit.cls = cls;
    unit.position = next_position_;
    unit.capacity = capacity;
    coaches_.push_back(unit);
    ++next_position_;
}

void TrainComposition::prepend_engine() {
    for (size_t i = 0; i < coaches_.size(); ++i) {
        coaches_[i].position = static_cast<uint16_t>(coaches_[i].position + 1);
    }
    CoachUnit engine;
    engine.code = "ENG";
    engine.cls = FareClass::Second;
    engine.position = 1;
    engine.capacity = 0;
    coaches_.insert(coaches_.begin(), engine);
    ++next_position_;
}

size_t TrainComposition::coach_count() const {
    return coaches_.size();
}

uint16_t TrainComposition::total_capacity() const {
    uint32_t total = 0;
    for (size_t i = 0; i < coaches_.size(); ++i) {
        total += coaches_[i].capacity;
    }
    if (total > 0xFFFFu) {
        return 0xFFFFu;
    }
    return static_cast<uint16_t>(total);
}

bool TrainComposition::position_of(const std::string &code,
                                   uint16_t *out) const {
    for (size_t i = 0; i < coaches_.size(); ++i) {
        if (coaches_[i].code == code) {
            if (out) {
                *out = coaches_[i].position;
            }
            return true;
        }
    }
    return false;
}

std::vector<CoachUnit> TrainComposition::coaches_of_class(
    FareClass cls) const {
    std::vector<CoachUnit> out;
    for (size_t i = 0; i < coaches_.size(); ++i) {
        if (coaches_[i].cls == cls) {
            out.push_back(coaches_[i]);
        }
    }
    return out;
}

std::string TrainComposition::layout_string() const {
    std::string out;
    for (size_t i = 0; i < coaches_.size(); ++i) {
        if (i > 0) {
            out += "-";
        }
        out += coaches_[i].code;
    }
    return out;
}

bool TrainComposition::reverse() {
    if (coaches_.empty()) {
        return false;
    }
    size_t lo = 0;
    size_t hi = coaches_.size() - 1;
    while (lo < hi) {
        CoachUnit tmp = coaches_[lo];
        coaches_[lo] = coaches_[hi];
        coaches_[hi] = tmp;
        ++lo;
        --hi;
    }
    for (size_t i = 0; i < coaches_.size(); ++i) {
        coaches_[i].position = static_cast<uint16_t>(i + 1);
    }
    return true;
}

const CoachUnit &TrainComposition::at(size_t i) const {
    if (i >= coaches_.size()) {
        return empty_;
    }
    return coaches_[i];
}

}
