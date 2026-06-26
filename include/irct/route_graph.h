#ifndef IRCT_ROUTE_GRAPH_H
#define IRCT_ROUTE_GRAPH_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace irct {

struct Edge {
    uint32_t to;
    uint32_t train_number;
    int minutes;
};

class RouteGraph {
public:
    explicit RouteGraph(uint32_t node_count);

    void add_edge(uint32_t from, uint32_t to, uint32_t train_number, int minutes);

    uint32_t node_count() const;

    size_t edge_count() const;

    const std::vector<Edge> &edges_from(uint32_t node) const;

    bool reachable(uint32_t from, uint32_t to) const;

    int shortest_minutes(uint32_t from, uint32_t to) const;

    size_t degree(uint32_t node) const;

private:
    std::vector<std::vector<Edge> > adj_;
    std::vector<Edge> empty_;
};

}

#endif
