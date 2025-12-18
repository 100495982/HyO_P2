// grafo.cpp
#include "grafo.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

void Grafo::loadGraph(std::string_view gr_file, std::string_view co_file) {
    // Reset state (in case reused)
    adjacency_list.clear();
    vertices.clear();
    vertex_index_map.clear();
    edge_count = 0;

    // 1) Load coordinates (.co) first [file:1]
    parseCoordinatesFile(co_file);

    // 2) Allocate adjacency list with contiguous indices
    adjacency_list.assign(vertices.size(), {});

    // 3) Load arcs (.gr) [file:1]
    parseGraphFile(gr_file);
}

void Grafo::parseCoordinatesFile(std::string_view filename) {
    std::ifstream file{std::string(filename)};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open coordinates file: " + std::string(filename));
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] != 'v') continue;

        // Format: v id longitude latitude  (both scaled by 1e6) [file:1]
        std::istringstream iss(line);
        char prefix;
        VertexID id;
        Coordinate lon;
        Coordinate lat;

        if (!(iss >> prefix >> id >> lon >> lat)) {
            continue; // ignore malformed lines
        }

        // Avoid duplicates if present
        if (vertex_index_map.find(id) != vertex_index_map.end()) {
            continue;
        }

        size_t idx = vertices.size();
        vertex_index_map[id] = idx;

        Vertex v;
        v.id = id;
        v.longitude = lon;
        v.latitude = lat;
        vertices.push_back(v);
    }
}

void Grafo::parseGraphFile(std::string_view filename) {
    std::ifstream file{std::string(filename)};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open graph file: " + std::string(filename));
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] != 'a') continue;

        // Format: a id1 id2 cost [file:1]
        std::istringstream iss(line);
        char prefix;
        VertexID u, v;
        Distance cost;

        if (!(iss >> prefix >> u >> v >> cost)) {
            continue; // ignore malformed lines
        }

        auto it_u = vertex_index_map.find(u);
        auto it_v = vertex_index_map.find(v);

        // Some datasets might contain arcs referencing nodes not present in .co; ignore them safely
        if (it_u == vertex_index_map.end() || it_v == vertex_index_map.end()) {
            continue;
        }

        adjacency_list[it_u->second].push_back(Edge{v, cost});
        edge_count++;
    }
}
