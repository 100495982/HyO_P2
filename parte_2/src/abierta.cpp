// abierta.cpp
#include "abierta.hpp"

void Abierta::push(const Node& node) {
    auto it = best_g_cost.find(node.vertex_id);

    // Only add if it's the first time or if g_cost is better
    if (it == best_g_cost.end() || node.g_cost < it->second) {
        best_g_cost[node.vertex_id] = node.g_cost;
        pq.push(node);
    }
}

Node Abierta::pop() {
    Node node = pq.top();
    pq.pop();
    return node;
}

bool Abierta::hasBetterPath(VertexID vertex, Distance g_cost) const {
    auto it = best_g_cost.find(vertex);
    return it != best_g_cost.end() && it->second <= g_cost;
}
