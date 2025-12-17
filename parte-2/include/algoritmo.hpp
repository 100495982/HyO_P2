// algoritmo.hpp
#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include "tipos.hpp"
#include "grafo.hpp"
#include "abierta.hpp"
#include "cerrada.hpp"
#include "heuristica.hpp"
#include <vector>
#include <chrono>

struct SolucionAStar {
    std::vector<VertexID> path;
    std::vector<Distance> edge_costs;
    Distance total_cost;
    size_t expansions;
    double time_seconds;
};

class Algoritmo {
private:
    const Grafo* grafo;
    Abierta abierta;
    Cerrada cerrada;
    Heuristica heuristica;
    size_t expansion_count = 0;

public:
    Algoritmo() = default;

    // A* Algorithm
    SolucionAStar solveAStar(
        const Grafo& g,
        VertexID start,
        VertexID goal);

    // BFS (Breadth-First Search) for comparison
    SolucionAStar solveBFS(
        const Grafo& g,
        VertexID start,
        VertexID goal);
};

#endif // ALGORITMO_HPP
