// algoritmo.cpp
#include "algoritmo.hpp"

#include <chrono>
#include <cmath>
#include <queue>
#include <vector>
#include <stack>

// constants for the haversine formula and the coordinate conversion
namespace {
constexpr double EARTH_RADIUS_M = 6371000.0;
constexpr double DEG_TO_RAD = 3.14159265358979323846 / 180.0;
constexpr double COORD_SCALE = 1e-6; // coordinates are degrees
}

// ------------------------------------------------------------
// A* Search Algorithm 
// ------------------------------------------------------------
SolucionAStar Algoritmo::solveAStar(const Grafo& g, VertexID start, VertexID goal) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolucionAStar res;
    res.total_cost = INFINITY_DIST;

    // validaion of the the existance of start and goal verctices
    if (!g.hasVertex(start) || !g.hasVertex(goal)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        return res;
    }

    // we reset the data strucutes for a new search
    abierta = Abierta();
    cerrada = Cerrada();
    size_t expansions = 0;

    // Heuristic function:
    // we use the haversine distance to calculate the distance in a straight line
    auto heuristic = [&](VertexID v) -> Distance {
        const Vertex& a = g.getVertex(v);
        const Vertex& b = g.getVertex(goal);


        // we convert the coordinates to radiands
        double lat1 = (static_cast<double>(a.latitude)  * COORD_SCALE) * DEG_TO_RAD;
        double lon1 = (static_cast<double>(a.longitude) * COORD_SCALE) * DEG_TO_RAD;
        double lat2 = (static_cast<double>(b.latitude)  * COORD_SCALE) * DEG_TO_RAD;
        double lon2 = (static_cast<double>(b.longitude) * COORD_SCALE) * DEG_TO_RAD;

        double dlat = lat2 - lat1;
        double dlon = lon2 - lon1;

        double s1 = std::sin(dlat / 2.0);
        double s2 = std::sin(dlon / 2.0);

        // haversine formula
        double aa = s1 * s1 + std::cos(lat1) * std::cos(lat2) * s2 * s2;
        double c = 2.0 * std::atan2(std::sqrt(aa), std::sqrt(1.0 - aa));
        double d = EARTH_RADIUS_M * c;

        if (d < 0.0) d = 0.0;
        return static_cast<Distance>(d);
    };

    // we initialize the search adding 
    cerrada.add(start, INVALID_VERTEX, 0);
    abierta.push(Node{start, 0, heuristic(start)});

    while (!abierta.empty()) {
    // pop the best candidate (node with the lowest f = g + h)
        Node current = abierta.pop();

        // we skip it if we have found a better path to this node already
        if (current.g_cost != cerrada.getGCost(current.vertex_id)) {
            continue;
        }

        expansions++;

        // we have reached the goal so we finalize thre results and we reconstruct the path
        if (current.vertex_id == goal) {
            auto t1 = std::chrono::high_resolution_clock::now();
            res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
            res.expansion_count = expansions;
            res.total_cost = current.g_cost;

            res.path = cerrada.reconstructPath(goal, start);
            res.costs = cerrada.getEdgeCosts(res.path);
            return res;
        }

        // we expand the current node
        for (const auto& edge : g.getAdyacentes(current.vertex_id)) {
            VertexID nb = edge.target;
            Distance new_g = current.g_cost + edge.cost;

            // relaxation: we update the path to neighbor if we have found a better one
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

// ------------------------------------------------------------
// BFS (not optimal for weighted graphs)
// ------------------------------------------------------------
SolucionAStar Algoritmo::solveBFS(const Grafo& g, VertexID start, VertexID goal) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolucionAStar res;
    res.total_cost = INFINITY_DIST;

    if (!g.hasVertex(start) || !g.hasVertex(goal)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        return res;
    }

    cerrada = Cerrada();
    std::queue<VertexID> q;

    cerrada.add(start, INVALID_VERTEX, 0);
    q.push(start);

    size_t expansions = 0;

    while (!q.empty()) {
        VertexID u = q.front();
        q.pop();
        expansions++;

        if (u == goal) break;

        Distance gu = cerrada.getGCost(u);

        for (const auto& e : g.getAdyacentes(u)) {
            VertexID v = e.target;
            // we only add it if the vertex has no been visited
            if (cerrada.getGCost(v) != INFINITY_DIST) continue; 
            cerrada.add(v, u, gu + e.cost);
            q.push(v);
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    res.expansion_count = expansions;

    if (cerrada.getGCost(goal) != INFINITY_DIST) {
        res.total_cost = cerrada.getGCost(goal);
        res.path = cerrada.reconstructPath(goal, start);
        res.costs = cerrada.getEdgeCosts(res.path);
    }

    return res;
}

// ------------------------------------------------------------
// DFS (not optimal for weighted graphs)
// ------------------------------------------------------------
SolucionAStar Algoritmo::solveDFS(const Grafo& g, VertexID start, VertexID goal) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolucionAStar res;
    res.total_cost = INFINITY_DIST;

    if (!g.hasVertex(start) || !g.hasVertex(goal)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        return res;
    }

    cerrada = Cerrada();
    std::stack<VertexID> st;

    cerrada.add(start, INVALID_VERTEX, 0);
    st.push(start);

    size_t expansions = 0;

    while (!st.empty()) {
        VertexID u = st.top();
        st.pop();
        expansions++;

        if (u == goal) break;

        Distance gu = cerrada.getGCost(u);

        for (const auto& e : g.getAdyacentes(u)) {
            VertexID v = e.target;
            // we only add it if the vertex has no been visited
            if (cerrada.getGCost(v) != INFINITY_DIST) continue;
            cerrada.add(v, u, gu + e.cost);
            st.push(v);
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    res.expansion_count = expansions;

    if (cerrada.getGCost(goal) != INFINITY_DIST) {
        res.total_cost = cerrada.getGCost(goal);
        res.path = cerrada.reconstructPath(goal, start);
        res.costs = cerrada.getEdgeCosts(res.path);
    }

    return res;
}

// ------------------------------------------------------------
// Dijkstra / Uniform-Cost Search
// ------------------------------------------------------------
SolucionAStar Algoritmo::solveDijkstra(const Grafo& g, VertexID start, VertexID goal) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolucionAStar res;
    res.total_cost = INFINITY_DIST;

    if (!g.hasVertex(start) || !g.hasVertex(goal)) {
        auto t1 = std::chrono::high_resolution_clock::now();
        res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
        return res;
    }

    // Min-heap based on g_cost
    struct QItem {
        VertexID v;
        Distance g;
        bool operator>(const QItem& o) const { return g > o.g; }
    };

    std::priority_queue<QItem, std::vector<QItem>, std::greater<QItem>> pq;

    cerrada = Cerrada();
    cerrada.add(start, INVALID_VERTEX, 0);
    pq.push({start, 0});

    size_t expansions = 0;

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        // skip if we have foundd a better path to cur.v already
        if (cur.g != cerrada.getGCost(cur.v)) continue;

        expansions++;

        if (cur.v == goal) {
            res.total_cost = cur.g;
            break;
        }

        for (const auto& e : g.getAdyacentes(cur.v)) {
            VertexID nb = e.target;
            Distance new_g = cur.g + e.cost;

            if (new_g < cerrada.getGCost(nb)) {
                cerrada.add(nb, cur.v, new_g);
                pq.push({nb, new_g});
            }
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    res.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
    res.expansion_count = expansions;

    if (res.total_cost != INFINITY_DIST) {
        res.path = cerrada.reconstructPath(goal, start);
        res.costs = cerrada.getEdgeCosts(res.path);
    }

    return res;
}
