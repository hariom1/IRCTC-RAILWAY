#include "irct/alias_resolver.h"

namespace irct {

AliasResolver::AliasResolver() {}

std::string AliasResolver::normalize(const std::string &name) {
    std::string out;
    for (size_t i = 0; i < name.size(); ++i) {
        char c = name[i];
        if (c >= 'a' && c <= 'z') {
            out.push_back(static_cast<char>(c - 'a' + 'A'));
        } else if (c >= 'A' && c <= 'Z') {
            out.push_back(c);
        } else if (c >= '0' && c <= '9') {
            out.push_back(c);
        } else if (c == ' ' || c == '.' || c == '-') {
            continue;
        }
    }
    return out;
}

void AliasResolver::add_alias(const std::string &alias,
                              const std::string &canonical) {
    Mapping mapping;
    mapping.alias = normalize(alias);
    mapping.canonical = canonical;
    mappings_.push_back(mapping);
}

std::string AliasResolver::resolve(const std::string &name) const {
    std::string key = normalize(name);
    for (size_t i = 0; i < mappings_.size(); ++i) {
        if (mappings_[i].alias == key) {
            return mappings_[i].canonical;
        }
    }
    return name;
}

bool AliasResolver::has_alias(const std::string &alias) const {
    std::string key = normalize(alias);
    for (size_t i = 0; i < mappings_.size(); ++i) {
        if (mappings_[i].alias == key) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> AliasResolver::aliases_of(
    const std::string &canonical) const {
    std::vector<std::string> out;
    for (size_t i = 0; i < mappings_.size(); ++i) {
        if (mappings_[i].canonical == canonical) {
            out.push_back(mappings_[i].alias);
        }
    }
    return out;
}

size_t AliasResolver::alias_count() const {
    return mappings_.size();
}

bool AliasResolver::same_station(const std::string &a,
                                 const std::string &b) const {
    return resolve(a) == resolve(b);
}

void AliasResolver::clear() {
    mappings_.clear();
}

}
