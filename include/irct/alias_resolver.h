#ifndef IRCT_ALIAS_RESOLVER_H
#define IRCT_ALIAS_RESOLVER_H

#include <cstdint>
#include <string>
#include <vector>

namespace irct {

class AliasResolver {
public:
    AliasResolver();

    void add_alias(const std::string &alias, const std::string &canonical);

    std::string resolve(const std::string &name) const;

    bool has_alias(const std::string &alias) const;

    std::vector<std::string> aliases_of(const std::string &canonical) const;

    size_t alias_count() const;

    static std::string normalize(const std::string &name);

    bool same_station(const std::string &a, const std::string &b) const;

    void clear();

private:
    struct Mapping {
        std::string alias;
        std::string canonical;
    };

    std::vector<Mapping> mappings_;
};

}

#endif
