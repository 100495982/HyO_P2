// cerrada.cpp
#include "cerrada.hpp"

#include <algorithm>
#include <stdexcept>

void Cerrada::add(VertexID vertex, VertexID parent_vertex, Distance cost) {
    visited.insert(vertex);
    parent[vertex] = parent_vertex;
    g_cost[vertex] = cost;
}

bool Cerrada::isVisited(VertexID vertex) const {
    return visited.find(vertex) != visited.end();
}

VertexID Cerrada::getParent(VertexID vertex) const {
    auto it = parent.find(vertex);
    if (it == parent.end()) return INVALID_VERTEX;
    return it->second;
}

Distance Cerrada::getGCost(VertexID vertex) const {
    auto it = g_cost.find(vertex);
    if (it == g_cost.end()) return INFINITY_DIST;
    return it->second;
}

std::vector<VertexID> Cerrada::reconstructPath(VertexID goal, VertexID start) const {
    std::vector<VertexID> path;

    if (!isVisited(goal)) return path;

    VertexID current = goal;
    path.push_back(current);

    // Follow parents until we reach start or fail
    while (current != start) {
        VertexID p = getParent(current);
        if (p == INVALID_VERTEX) {
            // Parent chain broken => no reconstructable path
            return {};
        }
        current = p;
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Distance> Cerrada::getEdgeCosts(const std::vector<VertexID>& path) const {
    std::vector<Distance> costs;
    if (path.size() < 2) return costs;

    costs.reserve(path.size() - 1);

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        VertexID u = path[i];
        VertexID v = path[i + 1];

        Distance gu = getGCost(u);
        Distance gv = getGCost(v);

        if (gu == INFINITY_DIST || gv == INFINITY_DIST || gv < gu) {
            // Costs not consistent with parent chain
            costs.push_back(INFINITY_DIST);
        } else {
            costs.push_back(gv - gu);
        }
    }

    return costs;
}
