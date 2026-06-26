#include "irct/distance_table.h"
namespace irct {
DistanceTable::DistanceTable(uint32_t stations) : n_(stations) {
    matrix_.assign(static_cast<size_t>(stations) * stations, 0);
}
void DistanceTable::set(uint32_t a, uint32_t b, uint32_t km) {
    if (a >= n_ || b >= n_) {
        return;
    }
    matrix_[static_cast<size_t>(a) * n_ + b] = km;
    matrix_[static_cast<size_t>(b) * n_ + a] = km;
}
uint32_t DistanceTable::get(uint32_t a, uint32_t b) const {
    if (a >= n_ || b >= n_) {
        return 0;
    }
    return matrix_[static_cast<size_t>(a) * n_ + b];
}
uint32_t DistanceTable::cumulative(const std::vector<uint32_t> &route) const {
    uint32_t total = 0;
    for (size_t i = 1; i < route.size(); ++i) {
        total += get(route[i - 1], route[i]);
    }
    return total;
}
bool DistanceTable::farthest_from(uint32_t origin, uint32_t *out) const {
    if (origin >= n_ || n_ == 0) {
        return false;
    }
    uint32_t best = origin;
    uint32_t best_km = 0;
    for (uint32_t i = 0; i < n_; ++i) {
        uint32_t d = get(origin, i);
        if (d > best_km) {
            best_km = d;
            best = i;
        }
    }
    if (out) {
        *out = best;
    }
    return true;
}
uint32_t DistanceTable::station_count() const { return n_; }
void DistanceTable::build_linear(const std::vector<uint32_t> &gaps) {
    uint32_t acc = 0;
    std::vector<uint32_t> positions;
    positions.push_back(0);
    for (size_t i = 0; i < gaps.size(); ++i) {
        acc += gaps[i];
        positions.push_back(acc);
    }
    for (uint32_t a = 0; a < n_ && a < positions.size(); ++a) {
        for (uint32_t b = 0; b < n_ && b < positions.size(); ++b) {
            uint32_t d = positions[a] > positions[b] ? positions[a] - positions[b]
                                                     : positions[b] - positions[a];
            set(a, b, d);
        }
    }
}
}
