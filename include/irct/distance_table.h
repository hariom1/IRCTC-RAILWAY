#ifndef IRCT_DISTANCE_TABLE_H
#define IRCT_DISTANCE_TABLE_H
#include <cstddef>
#include <cstdint>
#include <vector>
namespace irct {
class DistanceTable {
public:
    explicit DistanceTable(uint32_t stations);
    void set(uint32_t a, uint32_t b, uint32_t km);
    uint32_t get(uint32_t a, uint32_t b) const;
    uint32_t cumulative(const std::vector<uint32_t> &route) const;
    bool farthest_from(uint32_t origin, uint32_t *out) const;
    uint32_t station_count() const;
    void build_linear(const std::vector<uint32_t> &gaps);
private:
    uint32_t n_;
    std::vector<uint32_t> matrix_;
};
}
#endif
