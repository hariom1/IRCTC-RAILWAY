#ifndef IRCT_TRAIN_CLASS_H
#define IRCT_TRAIN_CLASS_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

enum class TrainType {
    Passenger,
    Express,
    Superfast,
    Rajdhani,
    Shatabdi,
    Duronto,
    Vande
};

struct TrainProfile {
    TrainType type;
    std::string name;
    uint16_t max_speed_kmph;
    bool pantry;
    bool fully_reserved;
    uint16_t reservation_charge;
};

class TrainClassCatalog {
public:
    TrainClassCatalog();

    void load_defaults();

    bool profile_for(TrainType type, TrainProfile *out) const;

    uint16_t superfast_surcharge(TrainType type) const;

    bool requires_reservation(TrainType type) const;

    size_t count() const;

    const char *type_name(TrainType type) const;

    TrainType classify_by_speed(uint16_t speed) const;

private:
    std::vector<TrainProfile> profiles_;
};

}

#endif
