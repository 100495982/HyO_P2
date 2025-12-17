// bfs.cpp - Fuerza bruta BFS para comparación
#include "algoritmo.hpp"
#include <queue>
#include <chrono>

SolucionAStar Algoritmo::solveBFS(const Grafo& g, VertexID start, VertexID goal) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    grafo = &g;
    expansion_count = 0;
    
    // Clear data structures
    abierta = Abierta();  // Reutiliza tu Abierta pero como FIFO
    cerrada = Cerrada();
    
    // BFS usa cola FIFO (sin heurística f = g + h)
    std::queue<Node> cola_fifo;
    
    // Initialize with start node
    cerrada.add(start, INVALID_VERTEX, 0);  // g_cost = 0
    cola_fifo.push({start, 0, 0});  // {vertex, g_cost, h=0}
    expansion_count = 1;
    
    while (!cola_fifo.empty()) {
        Node current = cola_fifo.front();
        cola_fifo.pop();
        
        // Skip if already visited with better cost
        if (cerrada.isVisited(current.vertex_id)) {
            continue;
        }
        
        // Mark as visited
        cerrada.add(current.vertex_id, INVALID_VERTEX, current.g_cost);
        expansion_count++;
        
        // Goal reached
        if (current.vertex_id == goal) {
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count() / 1000000.0;
            
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
        
        // Explore neighbors (BFS nivel por nivel)
        for (const auto& edge : grafo->getAdyacentes(current.vertex_id)) {
            VertexID neighbor = edge.target;
            Distance new_g_cost = current.g_cost + edge.cost;
            
            // Skip if already visited
            if (cerrada.isVisited(neighbor)) {
                continue;
            }
            
            // En BFS: solo agregar si NO visitado (sin check de coste mejor)
            cola_fifo.push({neighbor, new_g_cost, 0});  // h=0 siempre
        }
    }
    
    // No path found
    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time).count() / 1000000.0;
    
    return {
        {},
        {},
        INFINITY_DIST,
        expansion_count,
        elapsed
    };
}
