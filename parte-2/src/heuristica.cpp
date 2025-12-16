// heuristica.cpp
#include "heuristica.hpp"
#include <cmath>

Distance Heuristica::compute(VertexID current) const {
    if (!grafo || !grafo->hasVertex(current) || !grafo->hasVertex(goal_vertex)) {
        return 0;
    }

    const auto& curr_vertex = grafo->getVertex(current);
    const auto& goal = grafo->getVertex(goal_vertex);

    // Convert from × 10^6 to radians
    double lat1 = curr_vertex.latitude * TO_RADIANS;
    double lon1 = curr_vertex.longitude * TO_RADIANS;
    double lat2 = goal.latitude * TO_RADIANS;
    double lon2 = goal.longitude * TO_RADIANS;

    // Haversine formula
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = std::sin(dlat/2) * std::sin(dlat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon/2) * std::sin(dlon/2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    double distance = EARTH_RADIUS * c;

    return static_cast<Distance>(distance);
}
