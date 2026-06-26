#ifndef IRCT_TRAIN_NUMBER_H
#define IRCT_TRAIN_NUMBER_H

#include <cstdint>
#include <string>

namespace irct {

class TrainNumber {
public:
    TrainNumber();

    explicit TrainNumber(uint32_t number);

    bool is_valid() const;

    uint8_t leading_digit() const;

    bool is_passenger() const;

    bool is_express() const;

    bool is_superfast() const;

    bool is_suburban() const;

    uint32_t pair_number() const;

    std::string formatted() const;

    const char *category() const;

    uint32_t value() const;

private:
    uint32_t number_;
};

}

#endif
