#ifndef IRCT_GEO_H
#define IRCT_GEO_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace irct {
struct GeoPoint { double lat; double lon; };
class GeoIndex {
public:
    GeoIndex();
    uint32_t add(const std::string &code, double lat, double lon);
    double distance_km(uint32_t a, uint32_t b) const;
    bool nearest(double lat, double lon, uint32_t *out) const;
    std::vector<uint32_t> within_km(double lat, double lon, double radius) const;
    size_t size() const;
    const std::string &code_of(uint32_t id) const;
    GeoPoint point_of(uint32_t id) const;
    void clear();
private:
    static double haversine(double lat1, double lon1, double lat2, double lon2);
    std::vector<GeoPoint> points_;
    std::vector<std::string> codes_;
    std::string empty_;
};
}
#endif
