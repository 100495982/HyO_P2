// heuristica.hpp
// utiliza Haversine distance
#ifndef HEURISTICA_HPP
#define HEURISTICA_HPP

#include "tipos.hpp"
#include "grafo.hpp"

class Heuristica {
private:
    const Grafo* grafo;
    VertexID goal_vertex;

    // Constants for Haversine formula (Earth radius in meters)
    static constexpr double EARTH_RADIUS = 6371000.0;  // meters
    static constexpr double TO_RADIANS = M_PI / (180.0 * 1e6);

public:
    Heuristica() = default;

    void setGoal(const Grafo* g, VertexID goal) {
        grafo = g;
        goal_vertex = goal;
    }

    // Compute admissible heuristic (never overestimates)
    Distance compute(VertexID current) const;
};

#endif // HEURISTICA_HPP
