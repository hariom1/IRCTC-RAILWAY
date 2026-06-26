#include "irct/geo.h"
#include <cmath>
namespace irct {
GeoIndex::GeoIndex() {}
double GeoIndex::haversine(double lat1, double lon1, double lat2, double lon2) {
    const double r = 6371.0;
    const double pi = 3.14159265358979323846;
    double dlat = (lat2 - lat1) * pi / 180.0;
    double dlon = (lon2 - lon1) * pi / 180.0;
    double a = std::sin(dlat / 2.0) * std::sin(dlat / 2.0) +
               std::cos(lat1 * pi / 180.0) * std::cos(lat2 * pi / 180.0) *
               std::sin(dlon / 2.0) * std::sin(dlon / 2.0);
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return r * c;
}
uint32_t GeoIndex::add(const std::string &code, double lat, double lon) {
    GeoPoint p;
    p.lat = lat;
    p.lon = lon;
    uint32_t id = static_cast<uint32_t>(points_.size());
    points_.push_back(p);
    codes_.push_back(code);
    return id;
}
double GeoIndex::distance_km(uint32_t a, uint32_t b) const {
    if (a >= points_.size() || b >= points_.size()) {
        return -1.0;
    }
    return haversine(points_[a].lat, points_[a].lon, points_[b].lat, points_[b].lon);
}
bool GeoIndex::nearest(double lat, double lon, uint32_t *out) const {
    if (points_.empty()) {
        return false;
    }
    double best = -1.0;
    uint32_t best_id = 0;
    for (size_t i = 0; i < points_.size(); ++i) {
        double d = haversine(lat, lon, points_[i].lat, points_[i].lon);
        if (best < 0.0 || d < best) {
            best = d;
            best_id = static_cast<uint32_t>(i);
        }
    }
    if (out) {
        *out = best_id;
    }
    return true;
}
std::vector<uint32_t> GeoIndex::within_km(double lat, double lon, double radius) const {
    std::vector<uint32_t> result;
    for (size_t i = 0; i < points_.size(); ++i) {
        double d = haversine(lat, lon, points_[i].lat, points_[i].lon);
        if (d <= radius) {
            result.push_back(static_cast<uint32_t>(i));
        }
    }
    return result;
}
size_t GeoIndex::size() const { return points_.size(); }
const std::string &GeoIndex::code_of(uint32_t id) const {
    if (id >= codes_.size()) {
        return empty_;
    }
    return codes_[id];
}
GeoPoint GeoIndex::point_of(uint32_t id) const {
    if (id >= points_.size()) {
        GeoPoint z;
        z.lat = 0.0;
        z.lon = 0.0;
        return z;
    }
    return points_[id];
}
void GeoIndex::clear() {
    points_.clear();
    codes_.clear();
}
}
