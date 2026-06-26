#include "irct/intern.h"

namespace irct {

InternPool::InternPool() {}

uint32_t InternPool::intern(const std::string &s) {
    std::unordered_map<std::string, uint32_t>::iterator it = index_.find(s);
    if (it != index_.end()) {
        return it->second;
    }
    uint32_t id = static_cast<uint32_t>(values_.size());
    values_.push_back(s);
    index_.insert(std::make_pair(s, id));
    return id;
}

const std::string &InternPool::resolve(uint32_t id) const {
    if (id >= values_.size()) {
        return empty_;
    }
    return values_[id];
}

bool InternPool::contains(const std::string &s) const {
    return index_.find(s) != index_.end();
}

size_t InternPool::size() const {
    return values_.size();
}

void InternPool::clear() {
    values_.clear();
    index_.clear();
}

}
