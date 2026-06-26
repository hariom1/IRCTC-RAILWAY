#ifndef IRCT_COACH_LAYOUT_H
#define IRCT_COACH_LAYOUT_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
enum class BerthType { Lower, Middle, Upper, SideLower, SideUpper };
struct Berth { uint16_t number; BerthType type; bool occupied; };
class CoachLayout {
public:
    explicit CoachLayout(const std::string &code);
    void generate_sleeper(uint16_t bays);
    BerthType berth_type(uint16_t number) const;
    bool allocate(uint16_t number);
    void free(uint16_t number);
    uint16_t first_available(BerthType preferred) const;
    size_t total() const;
    size_t occupied() const;
    const std::string &code() const;
private:
    std::string code_;
    std::vector<Berth> berths_;
};
}
#endif
