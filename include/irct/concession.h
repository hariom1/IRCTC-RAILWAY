#ifndef IRCT_CONCESSION_H
#define IRCT_CONCESSION_H
#include <cstddef>
#include <cstdint>
#include <string>
namespace irct {
enum class ConcessionType {
    None,
    SeniorMale,
    SeniorFemale,
    Student,
    Disabled,
    Patient
};
class Concession {
public:
    Concession();
    uint32_t percent(ConcessionType type) const;
    uint32_t apply(uint32_t base_paise, ConcessionType type) const;
    bool eligible_by_age(ConcessionType type, uint8_t age) const;
    const char *label(ConcessionType type) const;
    uint32_t min_age(ConcessionType type) const;
private:
    uint32_t round_paise(uint32_t value) const;
};
}
#endif
