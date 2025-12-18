// algoritmo.cpp
#include "algoritmo.hpp"

#include <chrono>
#include <cmath>
#include <queue>
#include <vector>

namespace {
constexpr double EARTH_RADIUS_M = 6371000.0;
constexpr double DEG_TO_RAD = 3.14159265358979323846 / 180.0;
constexpr double COORD_SCALE = 1e-6; // coords are degrees * 1e6 in .co [file:1]
}

SolucionAStar Algoritmo::solveAStar(const Grafo& g, VertexID start, VertexID goal) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolucionAStar res;
    res.total_cost = INFINITY_DIST;

    // Quick validity check
    if (!g.hasVertex(start) || !g.hasVertex(goal)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        return res;
    }

    // Reset structures
    abierta = Abierta();
    cerrada = Cerrada();
    size_t expansions = 0;

    // Heuristic: Haversine distance (meters) using .co coords (degrees * 1e6) [file:1]
    auto heuristic = [&](VertexID v) -> Distance {
        const Vertex& a = g.getVertex(v);
        const Vertex& b = g.getVertex(goal);

        double lat1 = (static_cast<double>(a.latitude)  * COORD_SCALE) * DEG_TO_RAD;
        double lon1 = (static_cast<double>(a.longitude) * COORD_SCALE) * DEG_TO_RAD;
        double lat2 = (static_cast<double>(b.latitude)  * COORD_SCALE) * DEG_TO_RAD;
        double lon2 = (static_cast<double>(b.longitude) * COORD_SCALE) * DEG_TO_RAD;

        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;

        double s1 = std::sin(dlat / 2.0);
        double s2 = std::sin(dlon / 2.0);

        double aa = s1 * s1 + std::cos(lat1) * std::cos(lat2) * s2 * s2;
        double c = 2.0 * std::atan2(std::sqrt(aa), std::sqrt(1.0 - aa));
        double d = EARTH_RADIUS_M * c;

        if (d < 0.0) d = 0.0;
        return static_cast<Distance>(d);
    };

    // Init
    cerrada.add(start, INVALID_VERTEX, 0);
    abierta.push(Node{start, 0, heuristic(start)});

    while (!abierta.empty()) {
        Node current = abierta.pop();

        // Skip stale queue entries (we keep only best g in cerrada)
        if (current.g_cost != cerrada.getGCost(current.vertex_id)) {
            continue;
        }

        expansions++;

        if (current.vertex_id == goal) {
            auto t1 = std::chrono::high_resolution_clock::now();
            res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
            res.expansion_count = expansions;
            res.total_cost = current.g_cost;

            res.path = cerrada.reconstructPath(goal, start);
            res.costs = cerrada.getEdgeCosts(res.path);
            return res;
        }

        for (const auto& edge : g.getAdyacentes(current.vertex_id)) {
            VertexID nb = edge.target;
            Distance new_g = current.g_cost + edge.cost;

            // Relaxation: allow improvement even if nb was seen before
            Distance old_g = cerrada.getGCost(nb);
            if (new_g < old_g) {
                cerrada.add(nb, current.vertex_id, new_g);
                abierta.push(Node{nb, new_g, heuristic(nb)});
            }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    res.expansion_count = expansions;
    return res;
}
