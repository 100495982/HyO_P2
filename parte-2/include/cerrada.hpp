// cerrada.hpp
#ifndef CERRADA_HPP
#define CERRADA_HPP

#include "tipos.hpp"
#include <unordered_set>
#include <unordered_map>

class Cerrada {
private:
    std::unordered_set<VertexID> visited;
    std::unordered_map<VertexID, VertexID> parent;
    std::unordered_map<VertexID, Distance> g_cost;

public:
    void add(VertexID vertex, VertexID parent_vertex, Distance cost);
    bool isVisited(VertexID vertex) const;

    VertexID getParent(VertexID vertex) const;
    Distance getGCost(VertexID vertex) const;

    size_t size() const { return visited.size(); }
    std::vector<VertexID> reconstructPath(VertexID goal, VertexID start) const;
    std::vector<Distance> getEdgeCosts(const std::vector<VertexID>& path) const;
};

#endif // CERRADA_HPP
