// algoritmo.hpp
#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include "tipos.hpp"
#include "grafo.hpp"
#include "abierta.hpp"
#include "cerrada.hpp"

#include <vector>

struct SolucionAStar {
    std::vector<VertexID> path;
    std::vector<Distance> costs;
    Distance total_cost = INFINITY_DIST;
    size_t expansion_count = 0;
    double elapsed = 0.0;
};

class Algoritmo {
private:
    Abierta abierta;
    Cerrada cerrada;

public:
    Algoritmo() = default;

    SolucionAStar solveAStar(const Grafo& g, VertexID start, VertexID goal);
    SolucionAStar solveBFS(const Grafo& g, VertexID start, VertexID goal);
};

#endif // ALGORITMO_HPP
