#include "irct/concession_card.h"

namespace irct {

ConcessionCardRegistry::ConcessionCardRegistry() {}

ConcessionCard *ConcessionCardRegistry::find(const std::string &card_id) {
    for (size_t i = 0; i < cards_.size(); ++i) {
        if (cards_[i].card_id == card_id) {
            return &cards_[i];
        }
    }
    return nullptr;
}

const ConcessionCard *ConcessionCardRegistry::find(
    const std::string &card_id) const {
    for (size_t i = 0; i < cards_.size(); ++i) {
        if (cards_[i].card_id == card_id) {
            return &cards_[i];
        }
    }
    return nullptr;
}

bool ConcessionCardRegistry::issue(const std::string &card_id,
                                   const std::string &holder,
                                   ConcessionType type,
                                   uint16_t valid_until_year) {
    if (card_id.empty()) {
        return false;
    }
    if (find(card_id)) {
        return false;
    }
    ConcessionCard card;
    card.card_id = card_id;
    card.holder_name = holder;
    card.type = type;
    card.valid_until_year = valid_until_year;
    card.active = true;
    cards_.push_back(card);
    return true;
}

bool ConcessionCardRegistry::is_valid(const std::string &card_id,
                                      uint16_t current_year) const {
    const ConcessionCard *card = find(card_id);
    if (!card) {
        return false;
    }
    if (!card->active) {
        return false;
    }
    return card->valid_until_year >= current_year;
}

bool ConcessionCardRegistry::lookup(const std::string &card_id,
                                    ConcessionCard *out) const {
    const ConcessionCard *card = find(card_id);
    if (!card) {
        return false;
    }
    if (out) {
        *out = *card;
    }
    return true;
}

bool ConcessionCardRegistry::revoke(const std::string &card_id) {
    ConcessionCard *card = find(card_id);
    if (!card) {
        return false;
    }
    card->active = false;
    return true;
}

size_t ConcessionCardRegistry::active_cards() const {
    size_t total = 0;
    for (size_t i = 0; i < cards_.size(); ++i) {
        if (cards_[i].active) {
            ++total;
        }
    }
    return total;
}

size_t ConcessionCardRegistry::count() const {
    return cards_.size();
}

std::vector<ConcessionCard> ConcessionCardRegistry::cards_of_type(
    ConcessionType type) const {
    std::vector<ConcessionCard> out;
    for (size_t i = 0; i < cards_.size(); ++i) {
        if (cards_[i].type == type) {
            out.push_back(cards_[i]);
        }
    }
    return out;
}

size_t ConcessionCardRegistry::expired_count(uint16_t current_year) const {
    size_t total = 0;
    for (size_t i = 0; i < cards_.size(); ++i) {
        if (cards_[i].valid_until_year < current_year) {
            ++total;
        }
    }
    return total;
}

}
