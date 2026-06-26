#ifndef IRCT_LOYALTY_H
#define IRCT_LOYALTY_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class LoyaltyTier {
    Bronze,
    Silver,
    Gold,
    Platinum
};
struct LoyaltyAccount {
    std::string member_id;
    uint64_t points;
    uint64_t lifetime_points;
};
class LoyaltyProgram {
public:
    LoyaltyProgram();
    bool open_account(const std::string &member_id);
    bool earn(const std::string &member_id, uint32_t fare_paise);
    bool redeem(const std::string &member_id, uint64_t points);
    LoyaltyTier tier_of(const std::string &member_id) const;
    uint64_t balance(const std::string &member_id) const;
    uint32_t earn_rate(LoyaltyTier tier) const;
    size_t member_count() const;
    const char *tier_label(LoyaltyTier tier) const;
    uint64_t redeemable_value_paise(const std::string &member_id) const;
private:
    LoyaltyAccount *find(const std::string &member_id);
    const LoyaltyAccount *find(const std::string &member_id) const;
    LoyaltyTier tier_for_points(uint64_t lifetime) const;
    std::vector<LoyaltyAccount> accounts_;
};
}
#endif
