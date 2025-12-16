#ifndef TIPOS_HPP
#define TIPOS_HPP

#include <cstdint>
#include <vector>
#include <limits>

using VertexID = uint32_t;
using Distance = uint64_t;  // In meters
using Coordinate = int32_t; // Latitude/Longitude × 10^6

constexpr Distance INFINITY_DIST = std::numeric_limits<Distance>::max();
constexpr VertexID INVALID_VERTEX = std::numeric_limits<VertexID>::max();

struct Vertex {
    VertexID id;
    Coordinate latitude;   // × 10^6
    Coordinate longitude;  // × 10^6
};

struct Edge {
    VertexID target;
    Distance cost;
};

struct Node {
    VertexID vertex_id;
    Distance g_cost;        // Actual cost from start
    Distance h_cost;        // Heuristic estimate to goal

    Distance f_cost() const { return g_cost + h_cost; }

    // For priority queue ordering (min-heap)
    bool operator>(const Node& other) const {
        return f_cost() > other.f_cost();
    }
};

#endif // TIPOS_HPP
