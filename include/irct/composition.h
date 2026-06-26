#ifndef IRCT_COMPOSITION_H
#define IRCT_COMPOSITION_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/fare_table.h"

namespace irct {

struct CoachUnit {
    std::string code;
    FareClass cls;
    uint16_t position;
    uint16_t capacity;
};

class TrainComposition {
public:
    TrainComposition();

    void append(const std::string &code, FareClass cls, uint16_t capacity);

    void prepend_engine();

    size_t coach_count() const;

    uint16_t total_capacity() const;

    bool position_of(const std::string &code, uint16_t *out) const;

    std::vector<CoachUnit> coaches_of_class(FareClass cls) const;

    std::string layout_string() const;

    bool reverse();

    const CoachUnit &at(size_t i) const;

private:
    std::vector<CoachUnit> coaches_;
    uint16_t next_position_;
    CoachUnit empty_;
};

}

#endif
