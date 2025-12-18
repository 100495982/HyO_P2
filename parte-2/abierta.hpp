// abierta.hpp
#ifndef ABIERTA_HPP
#define ABIERTA_HPP

#include "tipos.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

class Abierta {
private:
    // Min-heap via std::greater<Node>, using Node::operator> from tipos.hpp
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    std::unordered_map<VertexID, Distance> best_g_cost;

public:
    void push(const Node& node);
    Node pop();

    bool empty() const { return pq.empty(); }
    size_t size() const { return pq.size(); }

    // True if we already have a path to 'vertex' with g <= g_cost
    bool hasBetterPath(VertexID vertex, Distance g_cost) const;
};

#endif // ABIERTA_HPP
