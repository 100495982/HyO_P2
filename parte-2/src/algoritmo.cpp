// algoritmo.cpp
#include "algoritmo.hpp"
#include <queue>
#include <limits>

// Implementación A*
SolucionAStar Algoritmo::solveAStar(
    const Grafo& g,
    VertexID start,
    VertexID goal) {

    auto start_time = std::chrono::high_resolution_clock::now();

    grafo = &g;
    expansion_count = 0;

    // Clear data structures
    abierta = Abierta();
    cerrada = Cerrada();
    heuristica.setGoal(grafo, goal);

    // Initialize with start node
    Distance h_start = heuristica.compute(start);
    abierta.push({start, 0, h_start});

    while (!abierta.empty()) {
        Node current = abierta.pop();

        // Skip if already visited
        if (cerrada.isVisited(current.vertex_id)) {
            continue;
        }

        // Mark as visited
        cerrada.add(current.vertex_id, INVALID_VERTEX, current.g_cost);
        expansion_count++;

        // Goal reached
        if (current.vertex_id == goal) {
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double>(end_time - start_time).count();

            std::vector<VertexID> path = cerrada.reconstructPath(goal, start);
            std::vector<Distance> costs = cerrada.getEdgeCosts(path);

            return {
                path,
                costs,
                current.g_cost,
                expansion_count,
                elapsed
            };
        }

        // Explore neighbors
        for (const auto& edge : grafo->getAdyacentes(current.vertex_id)) {
            VertexID neighbor = edge.target;
            Distance new_g_cost = current.g_cost + edge.cost;

            // Skip if already visited with same/better cost
            if (cerrada.isVisited(neighbor)) {
                continue;
            }

            if (!abierta.hasBetterPath(neighbor, new_g_cost)) {
                Distance h = heuristica.compute(neighbor);
                abierta.push({neighbor, new_g_cost, h});
            }
        }
    }

    // No path found
    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end_time - start_time).count();

    return {
        {},
        {},
        INFINITY_DIST,
        expansion_count,
        elapsed
    };
}


// Implementación Dijkstra (fuerza bruta)
SolucionAStar Algoritmo::solveDijkstra(const Grafo& g, VertexID start, VertexID goal) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    grafo = &g;
    expansion_count = 0;
    
    abierta = Abierta();
    cerrada = Cerrada();
    
    Distance h_start = 0;
    abierta.push({start, 0, h_start});
    
    while (!abierta.empty()) {
        Node current = abierta.pop();
        
        if (cerrada.isVisited(current.vertex_id)) {
            continue;
        }
        
        cerrada.add(current.vertex_id, INVALID_VERTEX, current.g_cost);
        expansion_count++;
        
        if (current.vertex_id == goal) {
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count() / 1000000.0;
            
            std::vector<VertexID> path = cerrada.reconstructPath(goal, start);
            std::vector<Distance> costs = cerrada.getEdgeCosts(path);
            
            return {path, costs, current.g_cost, expansion_count, elapsed};
        }
        
        for (const auto& edge : grafo->getAdyacentes(current.vertex_id)) {
            VertexID neighbor = edge.target;
            Distance new_g_cost = current.g_cost + edge.cost;
            
            if (cerrada.isVisited(neighbor)) continue;
            
            if (!abierta.hasBetterPath(neighbor, new_g_cost)) {
                abierta.push({neighbor, new_g_cost, 0});  // h=0
            }
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time).count() / 1000000.0;
    
    return {{}, {}, INFINITY_DIST, expansion_count, elapsed};
}