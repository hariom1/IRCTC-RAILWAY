#include "irct/loyalty.h"
namespace irct {
LoyaltyProgram::LoyaltyProgram() {}
LoyaltyAccount *LoyaltyProgram::find(const std::string &member_id) {
    for (size_t i = 0; i < accounts_.size(); ++i) {
        if (accounts_[i].member_id == member_id) {
            return &accounts_[i];
        }
    }
    return nullptr;
}
const LoyaltyAccount *LoyaltyProgram::find(const std::string &member_id) const {
    for (size_t i = 0; i < accounts_.size(); ++i) {
        if (accounts_[i].member_id == member_id) {
            return &accounts_[i];
        }
    }
    return nullptr;
}
bool LoyaltyProgram::open_account(const std::string &member_id) {
    if (member_id.empty() || find(member_id)) {
        return false;
    }
    LoyaltyAccount account;
    account.member_id = member_id;
    account.points = 0;
    account.lifetime_points = 0;
    accounts_.push_back(account);
    return true;
}
LoyaltyTier LoyaltyProgram::tier_for_points(uint64_t lifetime) const {
    if (lifetime >= 100000) {
        return LoyaltyTier::Platinum;
    }
    if (lifetime >= 50000) {
        return LoyaltyTier::Gold;
    }
    if (lifetime >= 10000) {
        return LoyaltyTier::Silver;
    }
    return LoyaltyTier::Bronze;
}
uint32_t LoyaltyProgram::earn_rate(LoyaltyTier tier) const {
    switch (tier) {
        case LoyaltyTier::Bronze:
            return 2;
        case LoyaltyTier::Silver:
            return 3;
        case LoyaltyTier::Gold:
            return 4;
        case LoyaltyTier::Platinum:
            return 6;
    }
    return 2;
}
bool LoyaltyProgram::earn(const std::string &member_id, uint32_t fare_paise) {
    LoyaltyAccount *account = find(member_id);
    if (!account) {
        return false;
    }
    LoyaltyTier tier = tier_for_points(account->lifetime_points);
    uint64_t rupees = fare_paise / 100u;
    uint64_t points = rupees * earn_rate(tier);
    account->points += points;
    account->lifetime_points += points;
    return true;
}
bool LoyaltyProgram::redeem(const std::string &member_id, uint64_t points) {
    LoyaltyAccount *account = find(member_id);
    if (!account) {
        return false;
    }
    if (account->points < points) {
        return false;
    }
    account->points -= points;
    return true;
}
LoyaltyTier LoyaltyProgram::tier_of(const std::string &member_id) const {
    const LoyaltyAccount *account = find(member_id);
    if (!account) {
        return LoyaltyTier::Bronze;
    }
    return tier_for_points(account->lifetime_points);
}
uint64_t LoyaltyProgram::balance(const std::string &member_id) const {
    const LoyaltyAccount *account = find(member_id);
    if (!account) {
        return 0;
    }
    return account->points;
}
size_t LoyaltyProgram::member_count() const {
    return accounts_.size();
}
uint64_t LoyaltyProgram::redeemable_value_paise(
    const std::string &member_id) const {
    uint64_t points = balance(member_id);
    return points * 25u;
}
const char *LoyaltyProgram::tier_label(LoyaltyTier tier) const {
    switch (tier) {
        case LoyaltyTier::Bronze:
            return "bronze";
        case LoyaltyTier::Silver:
            return "silver";
        case LoyaltyTier::Gold:
            return "gold";
        case LoyaltyTier::Platinum:
            return "platinum";
    }
    return "unknown";
}
}
