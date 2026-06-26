#include "irct/train_class.h"

namespace irct {

TrainClassCatalog::TrainClassCatalog() {}

void TrainClassCatalog::load_defaults() {
    profiles_.clear();

    TrainProfile passenger;
    passenger.type = TrainType::Passenger;
    passenger.name = "Passenger";
    passenger.max_speed_kmph = 60;
    passenger.pantry = false;
    passenger.fully_reserved = false;
    passenger.reservation_charge = 0;
    profiles_.push_back(passenger);

    TrainProfile express;
    express.type = TrainType::Express;
    express.name = "Express";
    express.max_speed_kmph = 90;
    express.pantry = false;
    express.fully_reserved = false;
    express.reservation_charge = 1500;
    profiles_.push_back(express);

    TrainProfile superfast;
    superfast.type = TrainType::Superfast;
    superfast.name = "Superfast Express";
    superfast.max_speed_kmph = 110;
    superfast.pantry = true;
    superfast.fully_reserved = false;
    superfast.reservation_charge = 3000;
    profiles_.push_back(superfast);

    TrainProfile rajdhani;
    rajdhani.type = TrainType::Rajdhani;
    rajdhani.name = "Rajdhani Express";
    rajdhani.max_speed_kmph = 130;
    rajdhani.pantry = true;
    rajdhani.fully_reserved = true;
    rajdhani.reservation_charge = 6000;
    profiles_.push_back(rajdhani);

    TrainProfile shatabdi;
    shatabdi.type = TrainType::Shatabdi;
    shatabdi.name = "Shatabdi Express";
    shatabdi.max_speed_kmph = 150;
    shatabdi.pantry = true;
    shatabdi.fully_reserved = true;
    shatabdi.reservation_charge = 6000;
    profiles_.push_back(shatabdi);

    TrainProfile duronto;
    duronto.type = TrainType::Duronto;
    duronto.name = "Duronto Express";
    duronto.max_speed_kmph = 130;
    duronto.pantry = true;
    duronto.fully_reserved = true;
    duronto.reservation_charge = 6000;
    profiles_.push_back(duronto);

    TrainProfile vande;
    vande.type = TrainType::Vande;
    vande.name = "Vande Bharat";
    vande.max_speed_kmph = 160;
    vande.pantry = true;
    vande.fully_reserved = true;
    vande.reservation_charge = 8000;
    profiles_.push_back(vande);
}

bool TrainClassCatalog::profile_for(TrainType type, TrainProfile *out) const {
    for (size_t i = 0; i < profiles_.size(); ++i) {
        if (profiles_[i].type == type) {
            if (out) {
                *out = profiles_[i];
            }
            return true;
        }
    }
    return false;
}

uint16_t TrainClassCatalog::superfast_surcharge(TrainType type) const {
    TrainProfile profile;
    if (!profile_for(type, &profile)) {
        return 0;
    }
    if (profile.max_speed_kmph >= 110) {
        return profile.reservation_charge;
    }
    return 0;
}

bool TrainClassCatalog::requires_reservation(TrainType type) const {
    TrainProfile profile;
    if (!profile_for(type, &profile)) {
        return false;
    }
    return profile.fully_reserved;
}

size_t TrainClassCatalog::count() const {
    return profiles_.size();
}

const char *TrainClassCatalog::type_name(TrainType type) const {
    switch (type) {
        case TrainType::Passenger:
            return "passenger";
        case TrainType::Express:
            return "express";
        case TrainType::Superfast:
            return "superfast";
        case TrainType::Rajdhani:
            return "rajdhani";
        case TrainType::Shatabdi:
            return "shatabdi";
        case TrainType::Duronto:
            return "duronto";
        case TrainType::Vande:
            return "vande bharat";
    }
    return "unknown";
}

TrainType TrainClassCatalog::classify_by_speed(uint16_t speed) const {
    if (speed >= 160) {
        return TrainType::Vande;
    }
    if (speed >= 150) {
        return TrainType::Shatabdi;
    }
    if (speed >= 130) {
        return TrainType::Rajdhani;
    }
    if (speed >= 110) {
        return TrainType::Superfast;
    }
    if (speed >= 90) {
        return TrainType::Express;
    }
    return TrainType::Passenger;
}

}
