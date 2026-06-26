#ifndef IRCT_CONCESSION_CARD_H
#define IRCT_CONCESSION_CARD_H

#include <cstdint>
#include <string>
#include <vector>

#include "irct/concession.h"

namespace irct {

struct ConcessionCard {
    std::string card_id;
    std::string holder_name;
    ConcessionType type;
    uint16_t valid_until_year;
    bool active;
};

class ConcessionCardRegistry {
public:
    ConcessionCardRegistry();

    bool issue(const std::string &card_id, const std::string &holder,
               ConcessionType type, uint16_t valid_until_year);

    bool is_valid(const std::string &card_id, uint16_t current_year) const;

    bool lookup(const std::string &card_id, ConcessionCard *out) const;

    bool revoke(const std::string &card_id);

    size_t active_cards() const;

    size_t count() const;

    std::vector<ConcessionCard> cards_of_type(ConcessionType type) const;

    size_t expired_count(uint16_t current_year) const;

private:
    ConcessionCard *find(const std::string &card_id);

    const ConcessionCard *find(const std::string &card_id) const;

    std::vector<ConcessionCard> cards_;
};

}

#endif
