#include "irct/journey_planner.h"

#include <limits>
#include <queue>

namespace irct {

JourneyPlanner::JourneyPlanner(const RouteGraph &graph, const FareTable &fares)
    : graph_(graph), fares_(fares), max_transfers_(4) {}

size_t JourneyPlanner::max_transfers() const {
    return max_transfers_;
}

void JourneyPlanner::set_max_transfers(size_t value) {
    max_transfers_ = value;
}

int JourneyPlanner::estimate_distance_km(int minutes) const {
    if (minutes <= 0) {
        return 0;
    }
    return (minutes * 60) / 60;
}

JourneyPlan JourneyPlanner::reconstruct(uint32_t origin, uint32_t destination,
                                        const std::vector<int> &prev,
                                        FareClass cls) const {
    JourneyPlan plan;
    plan.total_minutes = 0;
    plan.total_fare = 0;
    plan.found = false;

    std::vector<uint32_t> path;
    int cur = static_cast<int>(destination);
    while (cur != -1) {
        path.push_back(static_cast<uint32_t>(cur));
        if (static_cast<uint32_t>(cur) == origin) {
            break;
        }
        cur = prev[static_cast<size_t>(cur)];
    }
    if (path.empty() || path.back() != origin) {
        return plan;
    }

    for (size_t i = path.size(); i > 1; --i) {
        uint32_t a = path[i - 1];
        uint32_t b = path[i - 2];
        const std::vector<Edge> &edges = graph_.edges_from(a);
        const Edge *chosen = nullptr;
        for (size_t e = 0; e < edges.size(); ++e) {
            if (edges[e].to == b) {
                if (!chosen || edges[e].minutes < chosen->minutes) {
                    chosen = &edges[e];
                }
            }
        }
        if (!chosen) {
            return plan;
        }
        Leg leg;
        leg.from_station = a;
        leg.to_station = b;
        leg.train_number = chosen->train_number;
        leg.minutes = chosen->minutes;
        plan.legs.push_back(leg);
        plan.total_minutes += chosen->minutes;
    }

    int km = estimate_distance_km(plan.total_minutes);
    plan.total_fare = fares_.total(cls, static_cast<uint32_t>(km));
    plan.found = true;
    return plan;
}

JourneyPlan JourneyPlanner::plan(uint32_t origin, uint32_t destination,
                                 FareClass cls) const {
    JourneyPlan empty;
    empty.total_minutes = 0;
    empty.total_fare = 0;
    empty.found = false;

    uint32_t nodes = graph_.node_count();
    if (origin >= nodes || destination >= nodes) {
        return empty;
    }

    const int inf = std::numeric_limits<int>::max();
    std::vector<int> dist(nodes, inf);
    std::vector<int> prev(nodes, -1);
    typedef std::pair<int, uint32_t> Item;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item> > pq;
    dist[origin] = 0;
    pq.push(std::make_pair(0, origin));

    while (!pq.empty()) {
        Item top = pq.top();
        pq.pop();
        int d = top.first;
        uint32_t u = top.second;
        if (d > dist[u]) {
            continue;
        }
        const std::vector<Edge> &edges = graph_.edges_from(u);
        for (size_t e = 0; e < edges.size(); ++e) {
            const Edge &edge = edges[e];
            int w = edge.minutes > 0 ? edge.minutes : 0;
            int nd = d + w;
            if (nd < dist[edge.to]) {
                dist[edge.to] = nd;
                prev[edge.to] = static_cast<int>(u);
                pq.push(std::make_pair(nd, edge.to));
            }
        }
    }

    if (dist[destination] == inf) {
        return empty;
    }
    return reconstruct(origin, destination, prev, cls);
}

}
