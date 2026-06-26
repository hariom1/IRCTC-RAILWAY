#include "irct/train_number.h"

#include <cstdio>

namespace irct {

TrainNumber::TrainNumber() : number_(0) {}

TrainNumber::TrainNumber(uint32_t number) : number_(number) {}

bool TrainNumber::is_valid() const {
    return number_ >= 10000 && number_ <= 99999;
}

uint8_t TrainNumber::leading_digit() const {
    if (number_ < 10000) {
        return 0;
    }
    return static_cast<uint8_t>(number_ / 10000);
}

bool TrainNumber::is_passenger() const {
    uint8_t lead = leading_digit();
    return lead == 5;
}

bool TrainNumber::is_express() const {
    uint8_t lead = leading_digit();
    return lead == 1 || lead == 2;
}

bool TrainNumber::is_superfast() const {
    uint8_t lead = leading_digit();
    return lead == 1 && (number_ / 1000) % 10 == 2;
}

bool TrainNumber::is_suburban() const {
    uint8_t lead = leading_digit();
    return lead == 9;
}

uint32_t TrainNumber::pair_number() const {
    if (!is_valid()) {
        return 0;
    }
    if (number_ % 2 == 0) {
        return number_ - 1;
    }
    return number_ + 1;
}

std::string TrainNumber::formatted() const {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%05u", number_);
    return std::string(buf);
}

const char *TrainNumber::category() const {
    if (!is_valid()) {
        return "invalid";
    }
    if (is_suburban()) {
        return "suburban";
    }
    if (is_passenger()) {
        return "passenger";
    }
    if (is_express()) {
        return "express";
    }
    return "other";
}

uint32_t TrainNumber::value() const {
    return number_;
}

}
