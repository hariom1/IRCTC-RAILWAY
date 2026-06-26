#ifndef IRCT_INTERN_H
#define IRCT_INTERN_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace irct {

class InternPool {
public:
    InternPool();

    uint32_t intern(const std::string &s);

    const std::string &resolve(uint32_t id) const;

    bool contains(const std::string &s) const;

    size_t size() const;

    void clear();

private:
    std::vector<std::string> values_;
    std::unordered_map<std::string, uint32_t> index_;
    std::string empty_;
};

}

#endif
