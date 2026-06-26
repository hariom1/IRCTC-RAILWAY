#include "irct/route_graph.h"

#include <limits>
#include <queue>

namespace irct {

RouteGraph::RouteGraph(uint32_t node_count) {
    adj_.resize(node_count);
}

void RouteGraph::add_edge(uint32_t from, uint32_t to, uint32_t train_number,
                          int minutes) {
    if (from >= adj_.size() || to >= adj_.size()) {
        return;
    }
    Edge e;
    e.to = to;
    e.train_number = train_number;
    e.minutes = minutes;
    adj_[from].push_back(e);
}

uint32_t RouteGraph::node_count() const {
    return static_cast<uint32_t>(adj_.size());
}

size_t RouteGraph::edge_count() const {
    size_t total = 0;
    for (size_t i = 0; i < adj_.size(); ++i) {
        total += adj_[i].size();
    }
    return total;
}

const std::vector<Edge> &RouteGraph::edges_from(uint32_t node) const {
    if (node >= adj_.size()) {
        return empty_;
    }
    return adj_[node];
}

bool RouteGraph::reachable(uint32_t from, uint32_t to) const {
    if (from >= adj_.size() || to >= adj_.size()) {
        return false;
    }
    std::vector<bool> seen(adj_.size(), false);
    std::queue<uint32_t> q;
    q.push(from);
    seen[from] = true;
    while (!q.empty()) {
        uint32_t cur = q.front();
        q.pop();
        if (cur == to) {
            return true;
        }
        for (size_t i = 0; i < adj_[cur].size(); ++i) {
            uint32_t nxt = adj_[cur][i].to;
            if (!seen[nxt]) {
                seen[nxt] = true;
                q.push(nxt);
            }
        }
    }
    return false;
}

int RouteGraph::shortest_minutes(uint32_t from, uint32_t to) const {
    if (from >= adj_.size() || to >= adj_.size()) {
        return -1;
    }
    const int inf = std::numeric_limits<int>::max();
    std::vector<int> dist(adj_.size(), inf);
    typedef std::pair<int, uint32_t> Item;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item> > pq;
    dist[from] = 0;
    pq.push(std::make_pair(0, from));
    while (!pq.empty()) {
        Item top = pq.top();
        pq.pop();
        int d = top.first;
        uint32_t u = top.second;
        if (d > dist[u]) {
            continue;
        }
        if (u == to) {
            return d;
        }
        for (size_t i = 0; i < adj_[u].size(); ++i) {
            const Edge &e = adj_[u][i];
            int nd = d + (e.minutes > 0 ? e.minutes : 0);
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push(std::make_pair(nd, e.to));
            }
        }
    }
    if (dist[to] == inf) {
        return -1;
    }
    return dist[to];
}

size_t RouteGraph::degree(uint32_t node) const {
    if (node >= adj_.size()) {
        return 0;
    }
    return adj_[node].size();
}

}
